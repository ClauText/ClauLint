

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


#define ENTER '\n' // cf) std::endl

// ClauText
#define ARRAY_QUEUE std::deque // chk?
#define VECTOR std::vector
//int log_result = 0;
#include <wiz/ClauText.h>
// ~ClauText

namespace Lint {
	class Option
	{
	public:
		enum class Type_ { ANY, INT, FLOAT, QUOTED_STRING, STRING, DATETIME_A, DATETIME_B, DATETIME_C };
		enum class Id_ { NONE, ID, TOTAL_ID };
		enum class Optional_ { NONE, OPTIONAL_ };
		enum class OneMore_ { NONE, ONEMORE, JUSTONE };
		enum class Required_ { NONE, REQUIRED };
		enum class Quoted_ { NONE, QUOTED }; // REMOVAL?
		enum class Order_ { NONE, OFF, ON }; // for global??
	public:
		Type_ type;
		Id_ id;
		Optional_ optional;
		OneMore_ onemore;
		Required_ required;
		Quoted_ quoted;
		std::string event_ids;
	public:
		Option() : type(Type_::ANY), id(Id_::NONE), optional(Optional_::NONE),
			onemore(OneMore_::NONE), required(Required_::NONE), quoted(Quoted_::NONE)
		{
			//
		}
	public:
		Option & Type(Type_ type) {
			this->type = type;
			return *this;
		}
		Option & Id(Id_ id) {
			this->id = id;
			return *this;
		}
		Option & Optional(Optional_ optional) {
			this->optional = optional;
			return *this;
		}
		Option & OneMore(OneMore_ onemore) {
			this->onemore = onemore;
			return *this;
		}
		Option & Required(Required_ required) {
			this->required = required;
			return *this;
		}
		Option & Quoted(Quoted_ quoted) {
			this->quoted = quoted;
			return *this;
		}
		Option & Event(const std::string& event_id) {
			this->event_ids += " ";
			this->event_ids += event_id;
			return *this;
		}
	};

	Option OptionFrom(const std::string& option_str)
	{
		if (option_str.empty()) {
			return Option(); // or throw error?
		}

		Option option;
		std::string::size_type start = 0;
		std::string::size_type find_percent = std::string::npos; // % : delimeter.

		while ((find_percent = option_str.find('%', start)) != std::string::npos) {
			std::string::size_type end_ = option_str.find('%', find_percent + 1);
			start = find_percent; // ex) abc%id?
			if (end_ == std::string::npos) {
				end_ = option_str.size();
			}

			const std::string opt = option_str.substr(start, end_ - 1 - start + 1);

			if ("%int" == opt) {
				option.Type(Option::Type_::INT);
			}
			else if ("%float" == opt) {
				option.Type(Option::Type_::FLOAT);
			}
			else if ("%quoted_string" == opt) {
				option.Type(Option::Type_::QUOTED_STRING);
			}
			else if ("%string" == opt) {
				option.Type(Option::Type_::STRING);
			}
			else if ("%datetime_a" == opt) {
				option.Type(Option::Type_::DATETIME_A);
			}
			else if ("%datetime_b" == opt) {
				option.Type(Option::Type_::DATETIME_B);
			}
			else if ("%datetime_c" == opt) {
				option.Type(Option::Type_::DATETIME_C);
			}
			else if ("%id" == opt) {
				option.Id(Option::Id_::ID);
			}
			else if ("%any" == opt) {
				option.Type(Option::Type_::ANY);
			}
			else if ("%optional" == opt) {
				option.Optional(Option::Optional_::OPTIONAL_);
			}
			else if ("%one_more" == opt) { // x = { 1 2 3 4  } -> x = { %int%one_more%event_plus_test }
				option.OneMore(Option::OneMore_::ONEMORE);
			}
			else if ("%just_one" == opt) {
				option.OneMore(Option::OneMore_::JUSTONE);
			}
			else if ("%required" == opt) {
				option.Required(Option::Required_::REQUIRED);
			}
			else if ("%quoted" == opt) { // removal?
				option.Quoted(Option::Quoted_::QUOTED);
			}
			else if (wiz::String::startsWith(opt, "%event_")) { // size check?
				std::string event_name = wiz::String::substring(opt, 7);
				option.Event(std::move(event_name));
			}

			start = end_;
		}

		return option;
	}


	bool Check(wiz::load_data::UserType* schema_eventUT, wiz::load_data::UserType* schemaUT, const wiz::load_data::UserType* clautextUT)
	{
		// to do
		

		return true;
	}

	inline bool _Validate(const wiz::load_data::UserType& schemaFileUT, const wiz::load_data::UserType& clautextUT)
	{
		// 1. $로 시작하는 것들을 따로 분리해서 별도의 UserType에 저장한다.
			// 1-1. $Event -> 따로 뽑아낸 다음 Event로 rename?
			// 1-2. $변수들 -> 따로 뽑아낸 다음 변수들로 rename?
		// 2. $로 시작하지는 않는 것들은 data check 내용이다?
			// 2-1. %로 시작하는 것을 찾아서 각각 수행한다. 
		wiz::load_data::UserType schema_eventUT;
		wiz::load_data::UserType schemaUT;
		int itemCount = 0;
		int utCount = 0;

		for (int i = 0; i < schemaFileUT.GetIListSize(); ++i) {
			if (schemaFileUT.IsItemList(i)) { // itemtype
				const std::string name = wiz::ToString(schemaFileUT.GetItemList(itemCount).GetName());
				if (name.size() > 1 && name[0] == '$') {
					schema_eventUT.AddItem(name.substr(1), schemaFileUT.GetItemList(itemCount).Get(0));
				}
				else {
					schemaUT.AddItem(name, schemaFileUT.GetItemList(itemCount).Get(0));
				}
				itemCount++;
			}
			else { // usertype
				const std::string name = wiz::ToString(schemaFileUT.GetUserTypeList(utCount)->GetName());
				wiz::load_data::UserType temp = *schemaFileUT.GetUserTypeList(utCount);

				if (name.size() > 1 && name[0] == '$') {
					temp.SetName(wiz::ToString(temp.GetName()).substr(1));
					schema_eventUT.AddUserTypeItem(std::move(temp));
				}
				else {
					schemaUT.AddUserTypeItem(std::move(temp));
				}
				utCount++;
			}
		}

		const bool chk = Check(&schema_eventUT, &schemaUT, &clautextUT);

		//// debug
		//std::cout << schema_eventUT.ToString() << ENTER
		//	<< schemaUT.ToString() << ENTER;
		return chk;
	}

	inline bool Validate(const wiz::load_data::UserType& schema, const wiz::load_data::UserType& clautext)
	{
		return _Validate(schema, clautext);
	}


	// to do
	//std::pair<bool, std::string> MakeSchema(const std::string& clautext)
	//{
	//	// to do
	//}


	inline std::pair<bool, wiz::load_data::UserType> LoadFile(const std::string& fileName)
	{
		std::ifstream inFile(fileName);
		if (!inFile) {
			inFile.close();
			return { false, wiz::load_data::UserType() };
		}

		inFile.seekg(0, inFile.end);
		const unsigned long long length = inFile.tellg();
		inFile.seekg(0, inFile.beg);

		char* buffer = new char[length + 1]; // 

									   // read data as a block:
		inFile.read(buffer, length);

		std::pair<bool, wiz::load_data::UserType> result(true, std::string(buffer));
		delete[] buffer;
		inFile.close();

		return result;
	}
	inline bool SaveFile(const std::string& fileName, wiz::load_data::UserType* data)
	{
		wiz::load_data::LoadData::SaveWizDB(*data, fileName, "1");
		return true;
	}

}

int main(int argc, char* argv[])
{
	std::string option;
	wiz::load_data::UserType schema; // argv[3]?
	wiz::load_data::UserType clautext; // argv[2]
	std::string fileName; // to save

	// -v : version? - to do
	// -V : Validate.
	// -m : make schema.
	if (argc == 4 && 0 == strcmp("-V", argv[1])) {
		option = "-V";

		auto chk_clautext = Lint::LoadFile(argv[2]);
		auto chk_schema = Lint::LoadFile(argv[3]);

		if (chk_schema.first) {
			schema = std::move(chk_schema.second);
		}
		else {
			std::cout << "schema load fail" << ENTER;
			return 1;
		}
		if (chk_clautext.first) {
			clautext = std::move(chk_clautext.second);
		}
		else {
			std::cout << "clautext load fail" << ENTER;
			return 2;
		}
	}
	else if (argc == 4 && 0 == strcmp("-m", argv[1])) {
		option = "-m";

		auto chk_clautext = Lint::LoadFile(argv[2]);

		if (chk_clautext.first) {
			clautext = std::move(chk_clautext.second);
		}
		else {
			std::cout << "clautext load fail" << ENTER;
			return 3;
		}
		fileName = argv[3];
	}

	if (option == "-V") {
		auto chk = Lint::Validate(schema, clautext);
		if (chk) {
			std::cout << "success" << ENTER;;
		}
		else {
			std::cout << "fail" << ENTER;;
		}
	}
	else if (option == "-m") {
	//	auto chk = MakeSchema(clautext);
	//	if (chk.first) {
	//		schema = chk.second;

	//		if (SaveFile(fileName, schema)) {

	//		}
	//		else {
	//			std::cout << "save fail" << ENTER;
	//			return 4;
	//		}
			
	//		std::cout << "success" << ENTER;
	//	}
	//	else {
	//		std::cout << "fail" << ENTER;
	//	}
	}
	else {
		std::cout << "it is not valid option" << ENTER;
	}

	return 0;
}


