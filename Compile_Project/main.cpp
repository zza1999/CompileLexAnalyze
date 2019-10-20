//#include <bits/stdc++.h>
#include <stdexcept>
#include <string>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

namespace Compile_Lexer    /*�﷨����*/
{
	namespace Compile_Lexer_Impl
	/*����Ƕ�������ռ䣬�˿ռ���Ҫ�ǽ��дʱ��塢���Ͷ����Լ�hash����*/
	{
		constexpr const char* keyWords[] //�ؼ��ʱ�
		{
			"auto", "break", "case", "char", "const", "continue",
			"default", "do", "double", "else", "enum", "extern",
			"float", "for", "goto", "if", "inline", "int", "long",
			"register", "restrict", "return", "short", "signed",
			"sizeof", "static", "struct", "switch", "typedef",
			"union", "unsigned", "void", "volatile", "while"
		};

		constexpr const char* punctuators[] //��������ű�
		{
			"[", "]", "(", ")", "{", "}", ".", "->",
			"++", "--", "&", "*", "+", "-", "~", "!",
			"/", "%", "<<", ">>", "<", ">", "<=", ">=",
			"==", "!=", "^", "|", "&&", "||",
			"?", ":", ",", ";", "...",
			"=", "*=", "/=", "%=", "+=", "-=",
			"<<=", ">>=", "&=", "^=", "|="
		};

		constexpr const char* escapes[]//ת���ַ���
		{
			"\\'", "\\\"", "\\?", "\\\\",
			"\\a", "\\b", "\\f", "\\n",
			"\\r", "\\t", "\\v","\\0",
			"\\ddd","\\xhh"
		};

		bool isKeyword(const std::string& candi)//�ж��Ƿ�Ϊ�ؼ���
		{
			static std::unordered_set<std::string> set;
			if (set.empty())
			{

				auto length = sizeof(keyWords)
					/ sizeof(keyWords[0]);
				for (size_t i = 0; i < length; i++)//���β���
					set.emplace(keyWords[i]);
			}
			if (set.find(candi) != set.end())//ƥ��
				return true;
			return false;
		}

		bool isPunctuator(const std::string& candi)//�ж��Ƿ�Ϊ�������
		{
			static std::unordered_set<std::string> set;
			if (set.empty())
			{
				auto length = sizeof(punctuators)
					/ sizeof(punctuators[0]);
				for (size_t i = 0; i < length; i++)//���β���
					set.emplace(punctuators[i]);
			}
			if (set.find(candi) != set.end())//ƥ��
				return true;
			return false;
		}

		bool isEscape(const std::string& candi)//�ж��Ƿ�Ϊת���ַ�
		{
			static std::unordered_set<std::string> set;
			if (set.empty())
			{
				auto length = sizeof(escapes)
					/ sizeof(escapes[0]);
				for (size_t i = 0; i < length; i++)//���β���
					set.emplace(escapes[i]);
			}
			if (set.find(candi) != set.end())//ƥ��
				return true;
			return false;
		}

		bool isBlank(char ch)//�ж��Ƿ�Ϊ�ո�
		{
			return (ch == ' ' || ch == '\t' || ch == '\v'
				|| ch == '\r' || ch == '\n');
		}

		enum class tokenType  
		/*����ö�٣���keyword(�ؼ���),identifier(��־��),
		number(����),string(�ַ���)��punctuator(���㡢������)����
		������ɽ���ļǺ�Ҳ��Ϊ������*/
		{
			keyword,
			identifier,
			number,
			string,
			punctuator
		};

		struct tokenHasher
		{
			size_t operator()(tokenType const& obj) const
			{
				return std::underlying_type<
					Compile_Lexer::Compile_Lexer_Impl::tokenType>::type(obj);
			}
		};

		const std::string& getTokenStr(tokenType token) //�����ַ���
		{
			static const std::unordered_map<tokenType, std::string, tokenHasher>
				tokenMapper =                               //������ϣ��
			{
				{ tokenType::keyword, "keyword" },
				{ tokenType::identifier, "identifier" },
				{ tokenType::number, "number" },
				{ tokenType::string, "string" },
				{ tokenType::punctuator, "punctuator" }
			};
			return tokenMapper.at(token);
		}
	}

	//�ʷ��������ĺ���������Ϊ�����ַ���������ַ���
	void lexAnalyzing(std::istream& is,std::ostream& os)
	{
		using namespace Compile_Lexer_Impl;

		size_t lineNum = 1;//ͳ��������
		size_t charNum = 0;//ͳ�����ַ���
		std::unordered_map<tokenType, size_t, tokenHasher> wordCount;//�����ͼ�������hashmapʵ�֣�
		std::set<std::string> symbols;                               //������������г��ֵķ��ţ���־�������֣��ַ�����
		std::vector<std::pair<tokenType, std::string>> tokens;       //�������
		std::vector<std::pair<std::string, size_t>> errors;          //������
		const char endOfLine = '\n';                                 

		//��һ�д������seg�У����д�������
		auto eatLine = [&is, &endOfLine, &lineNum, &charNum](std::string& seg)
		{
			char ch;
			while (true)
			{
				if (!is.get(ch))
					break;
				charNum++;
				if (ch == endOfLine)
					break;
				seg += ch;
			}
			lineNum++;
		};

		//���ͳ��ִ�������ʵ��
		auto addToken = [&symbols, &tokens, &wordCount](tokenType token, std::string seg)
		{
			if (token == tokenType::identifier ||token == tokenType::number ||
				token == tokenType::string)
				symbols.emplace(seg);
			if (token != tokenType::punctuator)
				wordCount[token]++;
			tokens.emplace_back(token,std::move(seg));
		};

		//�ʷ��������岿�֣���ѭ��
		while (true)
		{
			std::string seg;//�������ʶ����ַ���
			char ch;        //��ǰʶ����ַ�

			if (!is.get(ch))//EOF
				break;
			charNum++;
			seg += ch;
			int forward = is.peek();//forward��ʾ����һ�����ַ���peek()Ϊɨ�裬��get()��������ȡ

			if (isBlank(ch))//�ո�
			{
				if (ch == endOfLine)
					lineNum++;
			}

			else if (isalpha(ch) || ch == '_')//��־����ؼ���
			{
				while (true)//��ѭ�����������з�ʽ����P64ҳ��ͬ
				{
					char ch = is.peek();
					if (!isalpha(ch) && !isdigit(ch) && ch != '_')
						break;
					seg += (char)is.get();
					charNum++;
				}

				//�ж��Ǳ�־�����ǹؼ���
				if (isKeyword(seg))
					addToken(tokenType::keyword, seg);
				else
					addToken(tokenType::identifier, seg);
			}

			else if (ch == '.' || isdigit(ch))//���֣�.���
			{
				bool isInt = false, isFlt = false, isPunc = false;//�ֱ��ж���������������.���
				try
				{
					if (ch == '.' ||
						ch != '0' || (ch == '0' && forward == '.'))
					{
						auto state = 0;
						if (ch == '.')
							state = 1;

						while (true)//��ѭ�����������з�ʽ����P64ҳ��ͬ
						{
							char forward = is.peek();
							switch (state)
							{
							case 0:
								if (isdigit(forward))
									;
								else if (forward == '.')
									state = 2;
								else if (forward == 'e' || forward == 'E')
									state = 3;
								else
									isInt = true;
								break;
							case 1:
								if (isdigit(forward))
									state = 2;
								else if (forward == '.')
									state = 6;
								else
									isPunc = true;
								break;
							case 2:
								if (isdigit(forward))
									;
								else if (forward == 'e' || forward == 'E')
									state = 3;
								else
									isFlt = true;
								break;
							case 3:
								if (forward == '+' || forward == '-')
									state = 4;
								else if (isdigit(forward))
									state = 5;
								else//�쳣�������쳣��Ϣ
									throw std::runtime_error(
										"ָ�������� "
										+ seg);
								break;
							case 4:
								if (isdigit(forward))
									state = 5;
								else//�쳣�������쳣��Ϣ
									throw std::runtime_error(
										"ָ�������� "
										+ seg);
								break;
							case 5:
								if (isdigit(forward))
									;
								else
									isFlt = true;
								break;
							case 6:
								if (forward == '.')
									state = 7;
								else//�쳣�������쳣��Ϣ
									throw std::runtime_error(
										"�Ƿ�����ʾ: ..");
								break;
							case 7:
								isPunc = true;
								break;
							default://�쳣�������쳣��Ϣ
								throw std::runtime_error(
									"�Զ�������������");
								break;
							}
							if (isPunc || isInt || isFlt)
								break;

							//getchar
							seg += (char)is.get();
							charNum++;
						}
					}
					
					else if (forward == 'X' || forward == 'x')//0x,0X��ʽ
					{
						//��ȡx���ٴ�������
						seg += (char)is.get();
						charNum++;

						isInt = true;
						while (true)
						{
							char forward = is.peek();
							if (!isdigit(forward)
								&& !(forward >= 'A' && forward <= 'F')
								&& !(forward >= 'a' && forward <= 'f'))
								break;

							seg += (char)is.get();
							charNum++;
						}

						if (seg.size() <= 2)//0x,0X��û�к����ַ�
							throw std::runtime_error("�Ƿ�16���Ʊ�ʾ");
					}

					else//�˽��Ʊ�ʾ
					{
						isInt = true;
						while (true)
						{
							char forward = is.peek();
							if (forward < '0' || forward > '7')
								break;

							seg += (char)is.get();
							charNum++;
						}
					}

					if (isPunc)//������
					{
						addToken(tokenType::punctuator, seg);
						continue;
					}

					//������ܳ��ֵĺ�׺��������
					auto state = 2;
					if (isInt)
						state = 3;

					while (true)
					{
						auto isEnd = false;
						char forward = is.peek();
						switch (state)
						{

						case 0:
							if (isalnum(forward))//����ĸ������
								;
							else//�Ƿ���׺
								throw std::runtime_error(
									"�Ƿ���׺: " + seg);
							break;

						case 1:
							if (isalnum(forward))
								state = 0;
							else
								isEnd = true;
							break;
						//�����׺
						case 2:
							if (forward == 'f' || forward == 'F' ||
								forward == 'l' || forward == 'L')
								state = 1;
							else if (isalnum(forward))
								state = 0;
							else
								isEnd = true;
							break;
						//���ͺ�׺
						case 3:
							if (forward == 'u' || forward == 'U')
								state = 4;
							else if (forward == 'l' || forward == 'L')
								state = 5;
							else if (isalnum(forward))
								state = 0;
							else
								isEnd = true;
							break;
						case 4:
							if (forward == 'l' || forward == 'L')
								state = 1;
							else if (isalnum(forward))
								state = 0;
							else
								isEnd = true;
							break;

						case 5:
							if (forward == 'u' || forward == 'U')
								state = 1;
							else if (isalnum(forward))
								state = 0;
							else
								isEnd = true;
							break;
						default:
							throw std::runtime_error(
								"�Զ�������������");
							break;
						}
						if (isEnd) break;

						seg += (char)is.get();
						charNum++;
					}
					addToken(tokenType::number, seg);
				}
				catch (const std::exception & ex)//���������Ϣ������ʶ����
				{
					errors.emplace_back(ex.what(), lineNum);
				}
			}

			else if (ch == '\'' || ch == '"')//�ַ����ַ���
			{
				auto chType = ch;//�����ж��Ƿ�ƥ��
				try
				{
					while (true)
					{
						if (!is.get(ch))
							break;
						seg += ch;
						charNum++;

						if (ch == chType)//ƥ��
							break;

						if (ch == endOfLine)
						{
							seg.pop_back();
							lineNum++;
							throw std::runtime_error(  //�����պϴ�
								"�ַ����ַ���δ�պϣ�" + seg);
						}

						if (ch == '\\')
						{
							std::string tmp(1, ch);
							is.get(ch);
							tmp += ch;
							charNum++;

							if (ch == endOfLine)
							{
								seg.pop_back();
								lineNum++;
							}

							else if (isEscape(tmp))//��ת���ַ�
								seg += ch;
							else
							{
								seg.pop_back();
								errors.emplace_back(
									"�Ƿ�ת���ַ��� " + tmp,
									lineNum);
							}
						}
					}
					if (chType == '\'')
					{
						if (seg.size() <= 2)
							throw std::runtime_error(
								"''���ַ�Ϊ��");
						addToken(tokenType::number, seg);
					}
					else
						addToken(tokenType::string, seg);
				}
				catch (const std::exception & ex)//�������
				{
					errors.emplace_back(ex.what(), lineNum);
				}
			}
			/*
			*��
			*ע
			*��
			*/
			else if (ch == '/' && forward == '*')
			{
				while (true)
				{
					if (!is.get(ch))
						break;
					charNum++;

					if (ch == endOfLine)
					{
						lineNum++;
						seg += "\\n";
					}
					else
						seg += ch;

					if (ch == '*' && is.peek() == '/')
					{
						seg += (char)is.get();
						charNum++;
						break;
					}
				}
			}
			//��ע��
			else if (ch == '/' && forward == '/')
			{
				eatLine(seg);
			}
			
			//�����궨��
			else if (ch == '#')
			{
				eatLine(seg);
			}

			//�������
			else
			{
				bool isPunc = isPunctuator(seg);
				while (true)
				{
					//ɨ��֮����ַ�
					char forward = is.peek();
					if (isBlank(forward) || isalnum(forward) || forward == EOF)
						break;

					if (!isPunctuator(seg + forward))
					    //��ǰ�ַ���֮ǰ�Ĵ����������������������
						break;

					seg += (char)is.get();
					charNum++;
				}

				if (isPunc)
					addToken(tokenType::punctuator, seg);
				else
					errors.emplace_back(
						"�Ƿ������������: " + seg, lineNum);
			}
		}

		//������������
		//ͳ�Ƶ��ַ���������
		os << "Total Lines: " << lineNum << std::endl
			<< "Total Chars: " << charNum << std::endl;

		//ͳ�Ƶĸ����ͣ���punctuator���ĸ���
		os << "\nTotal Words:\n";
		for (auto& word : wordCount)
			os << "["<< getTokenStr(word.first) << "]:\t"
			<< word.second << std::endl;
		//����������
		os << "\nTokens:\n";
		for (auto& token : tokens)
			os << "<"<< getTokenStr(token.first)<<std::setw(20) <<"\t"
			<< token.second << ">"<<std::endl;
		//�������
		if (!errors.empty())
		{
			os << "\nErrors:\n";
			for (auto& error : errors)
				os << "[" << error.second << "]\t"
				<< error.first << std::endl;
		}
	}
}

//������
int main(int argc, char* argv[])
{
	try
	{
		using namespace Compile_Lexer;

		//�����ļ��ַ�������������ļ��ַ������ô��ǲ���Ҫ����������
		if (argc < 2)
			throw std::runtime_error("�������٣�");

		std::ifstream ifs(argv[1]);
		std::ofstream ofs(argv[1] + std::string(".output.txt"));

		if (!ifs.is_open())
			throw std::runtime_error(std::string("�޷��򿪣�") +
				argv[1]);
		if (!ofs.is_open())
			throw std::runtime_error(std::string("�޷��򿪣�") +
				argv[1] + ".output.txt");

		//�ʷ������ؼ�����
		lexAnalyzing(ifs, ofs);
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	return 0;
}