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

namespace Compile_Lexer    /*语法分析*/
{
	namespace Compile_Lexer_Impl
	/*隐含嵌套命名空间，此空间主要是进行词表定义、类型定义以及hash表建立*/
	{
		constexpr const char* keyWords[] //关键词表
		{
			"auto", "break", "case", "char", "const", "continue",
			"default", "do", "double", "else", "enum", "extern",
			"float", "for", "goto", "if", "inline", "int", "long",
			"register", "restrict", "return", "short", "signed",
			"sizeof", "static", "struct", "switch", "typedef",
			"union", "unsigned", "void", "volatile", "while"
		};

		constexpr const char* punctuators[] //运算标点符号表
		{
			"[", "]", "(", ")", "{", "}", ".", "->",
			"++", "--", "&", "*", "+", "-", "~", "!",
			"/", "%", "<<", ">>", "<", ">", "<=", ">=",
			"==", "!=", "^", "|", "&&", "||",
			"?", ":", ",", ";", "...",
			"=", "*=", "/=", "%=", "+=", "-=",
			"<<=", ">>=", "&=", "^=", "|="
		};

		constexpr const char* escapes[]//转义字符表
		{
			"\\'", "\\\"", "\\?", "\\\\",
			"\\a", "\\b", "\\f", "\\n",
			"\\r", "\\t", "\\v","\\0",
			"\\ddd","\\xhh"
		};

		bool isKeyword(const std::string& candi)//判断是否为关键字
		{
			static std::unordered_set<std::string> set;
			if (set.empty())
			{

				auto length = sizeof(keyWords)
					/ sizeof(keyWords[0]);
				for (size_t i = 0; i < length; i++)//依次查找
					set.emplace(keyWords[i]);
			}
			if (set.find(candi) != set.end())//匹配
				return true;
			return false;
		}

		bool isPunctuator(const std::string& candi)//判断是否为运算标点符
		{
			static std::unordered_set<std::string> set;
			if (set.empty())
			{
				auto length = sizeof(punctuators)
					/ sizeof(punctuators[0]);
				for (size_t i = 0; i < length; i++)//依次查找
					set.emplace(punctuators[i]);
			}
			if (set.find(candi) != set.end())//匹配
				return true;
			return false;
		}

		bool isEscape(const std::string& candi)//判断是否为转义字符
		{
			static std::unordered_set<std::string> set;
			if (set.empty())
			{
				auto length = sizeof(escapes)
					/ sizeof(escapes[0]);
				for (size_t i = 0; i < length; i++)//依次查找
					set.emplace(escapes[i]);
			}
			if (set.find(candi) != set.end())//匹配
				return true;
			return false;
		}

		bool isBlank(char ch)//判断是否为空格
		{
			return (ch == ' ' || ch == '\t' || ch == '\v'
				|| ch == '\r' || ch == '\n');
		}

		enum class tokenType  
		/*类型枚举，分keyword(关键词),identifier(标志符),
		number(数字),string(字符串)和punctuator(运算、标点符号)五类
		最后生成结果的记号也分为这五类*/
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

		const std::string& getTokenStr(tokenType token) //类型字符串
		{
			static const std::unordered_map<tokenType, std::string, tokenHasher>
				tokenMapper =                               //建立哈希表
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

	//词法分析核心函数，参数为输入字符流和输出字符流
	void lexAnalyzing(std::istream& is,std::ostream& os)
	{
		using namespace Compile_Lexer_Impl;

		size_t lineNum = 1;//统计总行数
		size_t charNum = 0;//统计总字符数
		std::unordered_map<tokenType, size_t, tokenHasher> wordCount;//各类型计数器（hashmap实现）
		std::set<std::string> symbols;                               //保存程序中所有出现的符号（标志符，数字，字符串）
		std::vector<std::pair<tokenType, std::string>> tokens;       //分析结果
		std::vector<std::pair<std::string, size_t>> errors;          //错误处理
		const char endOfLine = '\n';                                 

		//将一行代码放入seg中，按行处理数据
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

		//类型出现次数计数实现
		auto addToken = [&symbols, &tokens, &wordCount](tokenType token, std::string seg)
		{
			if (token == tokenType::identifier ||token == tokenType::number ||
				token == tokenType::string)
				symbols.emplace(seg);
			if (token != tokenType::punctuator)
				wordCount[token]++;
			tokens.emplace_back(token,std::move(seg));
		};

		//词法分析主体部分，大循环
		while (true)
		{
			std::string seg;//存放正在识别的字符串
			char ch;        //当前识别的字符

			if (!is.get(ch))//EOF
				break;
			charNum++;
			seg += ch;
			int forward = is.peek();//forward表示看下一个的字符，peek()为扫描，而get()是真正获取

			if (isBlank(ch))//空格
			{
				if (ch == endOfLine)
					lineNum++;
			}

			else if (isalpha(ch) || ch == '_')//标志符或关键字
			{
				while (true)//大循环，具体运行方式与是P64页相同
				{
					char ch = is.peek();
					if (!isalpha(ch) && !isdigit(ch) && ch != '_')
						break;
					seg += (char)is.get();
					charNum++;
				}

				//判断是标志符还是关键字
				if (isKeyword(seg))
					addToken(tokenType::keyword, seg);
				else
					addToken(tokenType::identifier, seg);
			}

			else if (ch == '.' || isdigit(ch))//数字，.标点
			{
				bool isInt = false, isFlt = false, isPunc = false;//分别判断整数，浮点数，.标点
				try
				{
					if (ch == '.' ||
						ch != '0' || (ch == '0' && forward == '.'))
					{
						auto state = 0;
						if (ch == '.')
							state = 1;

						while (true)//大循环，具体运行方式与是P64页相同
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
								else//异常，返回异常信息
									throw std::runtime_error(
										"指数表达错误： "
										+ seg);
								break;
							case 4:
								if (isdigit(forward))
									state = 5;
								else//异常，返回异常信息
									throw std::runtime_error(
										"指数表达错误： "
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
								else//异常，返回异常信息
									throw std::runtime_error(
										"非法标点表示: ..");
								break;
							case 7:
								isPunc = true;
								break;
							default://异常，返回异常信息
								throw std::runtime_error(
									"自动机错误，请重试");
								break;
							}
							if (isPunc || isInt || isFlt)
								break;

							//getchar
							seg += (char)is.get();
							charNum++;
						}
					}
					
					else if (forward == 'X' || forward == 'x')//0x,0X形式
					{
						//获取x后再处理数字
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

						if (seg.size() <= 2)//0x,0X后没有合适字符
							throw std::runtime_error("非法16进制表示");
					}

					else//八进制表示
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

					if (isPunc)//非数字
					{
						addToken(tokenType::punctuator, seg);
						continue;
					}

					//处理可能出现的后缀冗杂问题
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
							if (isalnum(forward))//是字母和数字
								;
							else//非法后缀
								throw std::runtime_error(
									"非法后缀: " + seg);
							break;

						case 1:
							if (isalnum(forward))
								state = 0;
							else
								isEnd = true;
							break;
						//浮点后缀
						case 2:
							if (forward == 'f' || forward == 'F' ||
								forward == 'l' || forward == 'L')
								state = 1;
							else if (isalnum(forward))
								state = 0;
							else
								isEnd = true;
							break;
						//整型后缀
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
								"自动机错误，请重试");
							break;
						}
						if (isEnd) break;

						seg += (char)is.get();
						charNum++;
					}
					addToken(tokenType::number, seg);
				}
				catch (const std::exception & ex)//处理错误信息，并标识行数
				{
					errors.emplace_back(ex.what(), lineNum);
				}
			}

			else if (ch == '\'' || ch == '"')//字符和字符串
			{
				auto chType = ch;//用于判断是否匹配
				try
				{
					while (true)
					{
						if (!is.get(ch))
							break;
						seg += ch;
						charNum++;

						if (ch == chType)//匹配
							break;

						if (ch == endOfLine)
						{
							seg.pop_back();
							lineNum++;
							throw std::runtime_error(  //报不闭合错
								"字符或字符串未闭合：" + seg);
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

							else if (isEscape(tmp))//是转义字符
								seg += ch;
							else
							{
								seg.pop_back();
								errors.emplace_back(
									"非法转义字符： " + tmp,
									lineNum);
							}
						}
					}
					if (chType == '\'')
					{
						if (seg.size() <= 2)
							throw std::runtime_error(
								"''内字符为空");
						addToken(tokenType::number, seg);
					}
					else
						addToken(tokenType::string, seg);
				}
				catch (const std::exception & ex)//处理错误
				{
					errors.emplace_back(ex.what(), lineNum);
				}
			}
			/*
			*块
			*注
			*释
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
			//行注释
			else if (ch == '/' && forward == '/')
			{
				eatLine(seg);
			}
			
			//分析宏定义
			else if (ch == '#')
			{
				eatLine(seg);
			}

			//其他标点
			else
			{
				bool isPunc = isPunctuator(seg);
				while (true)
				{
					//扫描之后的字符
					char forward = is.peek();
					if (isBlank(forward) || isalnum(forward) || forward == EOF)
						break;

					if (!isPunctuator(seg + forward))
					    //当前字符与之前的串加起来不构成运算标点符号
						break;

					seg += (char)is.get();
					charNum++;
				}

				if (isPunc)
					addToken(tokenType::punctuator, seg);
				else
					errors.emplace_back(
						"非法标点或运算符号: " + seg, lineNum);
			}
		}

		//将分析结果输出
		//统计的字符数和行数
		os << "Total Lines: " << lineNum << std::endl
			<< "Total Chars: " << charNum << std::endl;

		//统计的各类型（除punctuator）的个数
		os << "\nTotal Words:\n";
		for (auto& word : wordCount)
			os << "["<< getTokenStr(word.first) << "]:\t"
			<< word.second << std::endl;
		//分析结果输出
		os << "\nTokens:\n";
		for (auto& token : tokens)
			os << "<"<< getTokenStr(token.first)<<std::setw(20) <<"\t"
			<< token.second << ">"<<std::endl;
		//错误输出
		if (!errors.empty())
		{
			os << "\nErrors:\n";
			for (auto& error : errors)
				os << "[" << error.second << "]\t"
				<< error.first << std::endl;
		}
	}
}

//主函数
int main(int argc, char* argv[])
{
	try
	{
		using namespace Compile_Lexer;

		//导入文件字符流并建立输出文件字符流，好处是不需要建立缓冲区
		if (argc < 2)
			throw std::runtime_error("参数过少！");

		std::ifstream ifs(argv[1]);
		std::ofstream ofs(argv[1] + std::string(".output.txt"));

		if (!ifs.is_open())
			throw std::runtime_error(std::string("无法打开：") +
				argv[1]);
		if (!ofs.is_open())
			throw std::runtime_error(std::string("无法打开：") +
				argv[1] + ".output.txt");

		//词法分析关键函数
		lexAnalyzing(ifs, ofs);
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	return 0;
}