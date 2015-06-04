/*
= Brainfuck

If you have gcc:

----
g++ -o brainfuck.exe brainfuck.cpp
brainfuck.exe helloworld.bf
----
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

/**
 * Primitive Brainfuck commands
 */
typedef enum { 
    INCREMENT, // +
    DECREMENT, // -
    SHIFT_LEFT, // <
    SHIFT_RIGHT, // >
    INPUT, // ,
    OUTPUT, // .
    ZERO
} Command;

// Forward references. Silly C++!
class CommandNode;
class Loop;
class Program;

/**
 * Visits?!? Well, that'd indicate visitors!
 * A visitor is an interface that allows you to walk through a tree and do stuff.
 */
class Visitor {
    public:
        virtual void visit(const CommandNode * leaf) = 0;
        virtual void visit(const Loop * loop) = 0;
        virtual void visit(const Program * program) = 0;
};

/**
 * The Node class (like a Java abstract class) accepts visitors, but since it's pure virtual, we can't use it directly.
 */
class Node {
    public:
        virtual void accept (Visitor *v) = 0;
};

/**
 * CommandNode publicly extends Node to accept visitors.
 * CommandNode represents a leaf node with a primitive Brainfuck command in it.
 */
class CommandNode : public Node {
    public:
        Command command;
        int count;
        CommandNode(char c, int count) {
            this->count = count;
            switch(c) {
                case '+': command = INCREMENT; break;
                case '-': command = DECREMENT; break;
                case '<': command = SHIFT_LEFT; break;
                case '>': command = SHIFT_RIGHT; break;
                case ',': command = INPUT; break;
                case '.': command = OUTPUT; break;
                case '0': command = ZERO; break;
            }
        }
        void accept (Visitor * v) {
            v->visit(this);
        }
};

class Container: public Node {
    public:
        vector<Node*> children;
        virtual void accept (Visitor * v) = 0;
};

/**
 * Loop publicly extends Node to accept visitors.
 * Loop represents a loop in Brainfuck.
 */
class Loop : public Container {
    public:
        void accept (Visitor * v) {
            v->visit(this);
        }
};

/**
 * Program is the root of a Brainfuck program abstract syntax tree.
 * Because Brainfuck is so primitive, the parse tree is the abstract syntax tree.
 */
class Program : public Container {
    public:
        void accept (Visitor * v) {
            v->visit(this);
        }
};

/**
 * Read in the file by recursive descent.
 * Modify as necessary and add whatever functions you need to get things done.
 */
void parse(fstream & file, Container * container) {
    char c;
    int count;
    while (file >> c) {
        Loop *loopNode;
        switch(c) {
            case '+': case '-': case '<': case '>': case ',': case '.':
                count = 1;
                while( (char) file.peek() == c ) {
                    file >> c;
                    count++;
                }
                container->children.push_back(new CommandNode(c, count)); 
                break;
            case '[':
                loopNode = new Loop;
                parse(file, loopNode);
                if (loopNode->children.size() == 1) { // loop has one child
                    CommandNode* frontNode = (CommandNode*) loopNode->children.front();
                    if ( frontNode->count == 1 && (frontNode->command == INCREMENT 
                                               ||  frontNode->command == DECREMENT  ) ){ // if loop child is a single plus or minus
                        container->children.push_back(new CommandNode('0', 1));  // add single zero CommandNode instead of loop
                        continue;
                    }
                }  

                container->children.push_back(loopNode);
                break;
            case ']':
                return;
        }
    }
}

/**
 * A printer for Brainfuck abstract syntax trees.
 * As a visitor, it will just print out the commands as is.
 * For Loops and the root Program node, it walks through all the children.
 */
class Printer : public Visitor {
    public:
        void visit(const CommandNode * leaf) {
            for(int i = 0; i < leaf->count; i++) {
                switch (leaf->command) {
                    case INCREMENT:   cout << '+'; break;
                    case DECREMENT:   cout << '-'; break;
                    case SHIFT_LEFT:  cout << '<'; break;
                    case SHIFT_RIGHT: cout << '>'; break;
                    case INPUT:       cout << ','; break;
                    case OUTPUT:      cout << '.'; break;
                    case ZERO:        cout << "[+]"; break;
                }
            }
        }
        void visit(const Loop * loop) {
            cout << '[';
            for (vector<Node*>::const_iterator it = loop->children.begin(); it != loop->children.end(); ++it) {
                (*it)->accept(this);
            }
            cout << ']';
        }
        void visit(const Program * program) {
            for (vector<Node*>::const_iterator it = program->children.begin(); it != program->children.end(); ++it) {
                (*it)->accept(this);
            }
            cout << '\n';
        }
};

class Compiler : public Visitor {
    public:
        void visit(const CommandNode * leaf) {
            switch (leaf->command) {
                case INCREMENT:   cout << "box[i] += " << leaf->count << ";\n"; break;
                case DECREMENT:   cout << "box[i] -= " << leaf->count << ";\n"; break;
                case SHIFT_LEFT:  cout << "i -= " << leaf->count << ";\n"; break;
                case SHIFT_RIGHT: cout << "i += " << leaf->count << ";\n"; break;
                case INPUT:       
                    for(int i = 0; i < leaf->count; i++) {
                        cout << "box[i] = s.next().charAt(0);\n"; 
                    }
                    break;
                case OUTPUT:
                    for(int i = 0; i < leaf->count; i++) {
                        cout << "System.out.print(box[i]);\n"; 
                    }
                    break;
                case ZERO: cout << "box[i] = 0;\n"; break;
            }
        }
        void visit(const Loop * loop) {
            cout << "while(box[i] != 0) {\n";
            for (vector<Node*>::const_iterator it = loop->children.begin(); it != loop->children.end(); ++it) {
                (*it)->accept(this);
            }
            cout << "}\n";
        }
        void visit(const Program * program) {
            cout << "import java.util.Scanner;\n\nclass Helloworld {\npublic static void main(String[] args) {\nchar[] box = new char[30000];\nint i = 0;\nScanner s = new Scanner(System.in);\n";
            for (vector<Node*>::const_iterator it = program->children.begin(); it != program->children.end(); ++it) {
                (*it)->accept(this);
            }
            cout << "}\n}\n";
        }
};

class Interpreter : public Visitor {
    char memory[30000];
    int pointer;
    public:
        void visit(const CommandNode * leaf) {
            for(int i = 0; i < leaf->count; i++) {
                switch (leaf->command) {
                    case INCREMENT:
                        memory[pointer]++;
                        break;
                    case DECREMENT:
                        memory[pointer]--;
                        break;
                    case SHIFT_LEFT:
                        pointer--;
                        break;
                    case SHIFT_RIGHT:
                        pointer++;
                        break;
                    case INPUT:
                        cin >> memory[pointer];
                        break;
                    case OUTPUT:
                        cout << memory[pointer];
                        break;
                    case ZERO:
                        memory[pointer] = 0;
                        break;
                }
            }
        }
        void visit(const Loop * loop) {
            while (memory[pointer] != 0) {
                for (vector<Node*>::const_iterator it = loop->children.begin(); it != loop->children.end(); ++it) {
                    (*it)->accept(this);
                }
            }
        }
        void visit(const Program * program) {
            // zero init the memory array
            // set pointer to zero
            memset(memory, 0, 30000);
            pointer = 0;
            for (vector<Node*>::const_iterator it = program->children.begin(); it != program->children.end(); ++it) {
                (*it)->accept(this);
            }
        }
};

int main(int argc, char *argv[]) {
    fstream file;
    Program program;
    Printer printer;
    Interpreter interpreter;
    Compiler compiler;
    if (argc == 1) {
        cout << argv[0] << ": No input files." << endl;
    } else if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            file.open(argv[i], fstream::in);
            parse(file, & program);
            // program.accept(&printer);
            // program.accept(&interpreter);
            program.accept(&compiler);
            file.close();
        }
    }
}