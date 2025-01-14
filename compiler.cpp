#include<iostream>
#include<vector>
#include<map>
#include<cctype>
#include<string>

using namespace std;

enum TokenType{
	T_INT,T_ID,T_NUM,T_IF,T_ELSE,T_RETURN,T_ASSIGN,T_PLUS,T_MINUS,T_NUL,T_MUL,
	T_DIV,T_LPAREN,T_RPAREN,T_LBRACE,T_RBRACE,T_SEMICOLON,T_GT,T_EOF
};

struct Token{
	TokenType type;
	string value;
};

class lexer{
	private:
		string src;
		size_t pos;
	public: 
		lexer(const string &src){
			this->src = src;
			this->pos = 0;
		}
		string consumeNumber(){
			size_t start = pos;
			while(pos<src.size() && isdigit(src[pos])) pos++;
			return src.substr(start,pos-start);
		}
		string consumeWord(){
			size_t start = pos;
			while(pos<src.size() && isalnum(src[pos])) pos++;
			return src.substr(start,pos-start);
		}
		
		vector<Token> tokenizer(){
			vector<Token> tokens;
			while(pos<src.size()){
				char current=src[pos];
				if(isspace(current)){
					pos++;
					continue;
				}
				if(isdigit(current)){
					tokens.push_back(Token{T_NUM,consumeNumber()});
					continue;
				}
				if(isalpha(current)){
					string word = consumeWord();
					if(word == "int") tokens.push_back(Token{T_INT,word});
					else if(word == "if") tokens.push_back(Token{T_IF,word});
					else if(word == "else") tokens.push_back(Token{T_ELSE,word});
					else if(word == "return") tokens.push_back(Token{T_RETURN,word});
					else tokens.push_back(Token{T_ID,word});
					continue;
				}
				switch(current){
					case'=':tokens.push_back(Token{T_ASSIGN,"="});break;
					case'+':tokens.push_back(Token{T_PLUS,"+"});break;
					case'-':tokens.push_back(Token{T_MINUS,"-"});break;
					case'*':tokens.push_back(Token{T_MUL,"*"});break;
					case'/':tokens.push_back(Token{T_DIV,"/"});break;
					case'(':tokens.push_back(Token{T_LPAREN,"("});break;
					case')':tokens.push_back(Token{T_RPAREN,")"});break;
					case'{':tokens.push_back(Token{T_LBRACE,"{"});break;
					case'}':tokens.push_back(Token{T_RBRACE,"}"});break;
					case';':tokens.push_back(Token{T_SEMICOLON,";"});break;
					case'>':tokens.push_back(Token{T_GT,">"});break;
					default:cout<<"Unexpected character: "<<current<<endl; exit(1);
					
				}
				pos++;			
			}
			tokens.push_back(Token{T_EOF,""});
			return tokens;
		}
};

class Parser{
	private:
		vector<Token>tokens;
		size_t pos;
	public:
		Parser(const vector<Token> &tokens){
			this->tokens = tokens;
			this->pos = 0;
		}
		void parseProgram(){
			while(tokens[pos].type != T_EOF){
				parseStatement();
			}
			cout<<"Parsing completed successfully! No Syntax Error"<<endl;
		}
		void parseStatement(){
			if(tokens[pos].type == T_INT){
				parseDeclaration();
			}else if(tokens[pos].type == T_ID){
				parseAssignment();
			}else if(tokens[pos].type == T_IF){
				parseIfStatement();
			}else if(tokens[pos].type == T_RETURN){
				parseReturnStatement();
			}else if(tokens[pos].type == T_LBRACE){
				parseBlock();
			}else{
				cout<<"Sytax error: unexpected token "<<tokens[pos].value<<endl;
				exit(1);
			}
		}
		
		void parseBlock(){
			expect(T_LBRACE);
			while(tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF){
				parseStatement();
			}
			expect(T_RBRACE);
		}
		void parseDeclaration(){
			expect(T_INT);
			expect(T_ID);
			expect(T_SEMICOLON);
		}
		void parseAssignment(){
			expect(T_ID);
			expect(T_ASSIGN);
			parseExpression();
			expect(T_SEMICOLON);
			
		}
		void parseIfStatement(){
			expect(T_IF);
			expect(T_LPAREN);
			parseExpression();
			expect(T_RPAREN);
			parseStatement();
			if(tokens[pos].type ==T_ELSE){
				expect(T_ELSE);
				parseStatement();
			}
		}
		void parseReturnStatement(){
			expect(T_RETURN);
			parseExpression();
			expect(T_SEMICOLON);
			
		}
		void parseExpression(){
			parseTerm();
			while(tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS){
				pos++;
				parseTerm();
			}
			if(tokens[pos].type == T_GT){
				pos++;
				parseExpression();
			}
		}
		void parseTerm(){
			parseFactor();
			while(tokens[pos].type == T_MUL || tokens[pos].type == T_DIV){
				pos++;
				parseFactor();
			}
		}
		void parseFactor(){
			if(tokens[pos].type == T_NUM || tokens[pos].type == T_ID){
				pos++;
			}
			else if(tokens[pos].type == T_LPAREN){
				expect(T_LPAREN);
				parseExpression();
				expect(T_RPAREN);
			}
			else{
				cout<<"Sytax error: unexprected token "<<tokens[pos].value<<endl;
				exit(1);
			}
		}
		void expect(TokenType type){
			if(tokens[pos].type == type){
				pos++;
			}
			else{
				cout<<"Syntax error: expected "<<type<<" but found "<<tokens[pos].value<<endl;
				exit(1);
			}
		}
		
		
};

int main(){
	string input = R"(
		int a;
		a = 5;
		int b;
		b = a + 10;
		if(b>10){
			return b;
		}else{
			return 0;}
	)";
	lexer Lexer(input);
	vector<Token> tokens = Lexer.tokenizer();
	
	Parser parser(tokens);
	parser.parseProgram();
	
	return 0;
}




