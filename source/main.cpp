

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
	wiz::StringBuilder builder(102400);
	bool cout_off = false; // print yes or no?
	wiz::Option opt;

	// justOne + optional_ -> no ok?
	// onemore + optional_ -> no ok?
	class Option
	{
	public:
		enum class Type_ { ANY, INT, FLOAT, QUOTED_STRING, STRING, DATETIME, DATETIME_A, DATETIME_B };

		//enum class Id_ { NONE, ID, TOTAL_ID };
		enum class Optional_ { NONE, OPTIONAL_ };
		enum class OneMore_ { NONE, ONEMORE, JUSTONE };
		enum class Required_ { NONE, REQUIRED };

		enum class Order_ { NONE, OFF, ON }; // with global??
	public:
		Type_ type;
		//Id_ id;
		Optional_ optional;
		OneMore_ onemore;
		Required_ required;
		std::string event_ids;

		std::string prefix;
	public:
		Option() : type(Type_::ANY), optional(Optional_::NONE),//id(Id_::NONE), 
			onemore(OneMore_::NONE), required(Required_::NONE)
		{
			//
		}
	public:
		Option & Type(Type_ type) {
			this->type = type;
			return *this;
		}
		//Option & Id(Id_ id) {
		//	this->id = id;
		//	return *this;
		//}
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
		Option & Event(const std::string& event_id) {
			this->event_ids += " ";
			this->event_ids += event_id;
			return *this;
		}
	public:
		// check valid function?
		bool IsValid() const {

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

		if (option_str.find('%', start) == std::string::npos)
		{
			option.prefix = option_str;
		}
		while ((find_percent = option_str.find('%', start)) != std::string::npos) 
		{
			std::string::size_type end_ = option_str.find('%', find_percent + 1);
			start = find_percent; // ex) abc%id?
			if (end_ == std::string::npos) {
				end_ = option_str.size();
			}
			if (start != find_percent) {
				option.prefix = option_str.substr(0, end_ - 1 - start + 1);
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
			else if ("%datetime" == opt) {
				option.Type(Option::Type_::DATETIME);
			}
			else if ("%datetime_a" == opt) {
				option.Type(Option::Type_::DATETIME_A);
			}
			else if ("%datetime_b" == opt) {
				option.Type(Option::Type_::DATETIME_B);
			}
		//	else if ("%id" == opt) {
		//		option.Id(Option::Id_::ID);
		//	}
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
			else if (wiz::String::startsWith(opt, "%event_")) { // size check?
				std::string event_name = wiz::String::substring(opt, 7);
				option.Event(std::move(event_name));
			}

			start = end_;
		}

		return option;
	}

	// chk valid in here!
	inline bool OptionDoA(const Option& option, const std::string& str) 
	{
		if (option.prefix.empty() == false &&
			option.prefix == str) {
			return true;
		}
		else if (option.prefix.empty() == false &&
			option.prefix != str) {
			return false;
		}
		switch (option.type) {
		case Option::Type_::ANY:
			// pass
			break;
		case Option::Type_::INT:
			if (wiz::load_data::Utility::IsInteger(str)) {
				//
			}
			else {
				std::cout << str << " is not integer" << ENTER;
				return false;
			}
			break;
		case Option::Type_::FLOAT:
			if (wiz::load_data::Utility::IsDouble(str)) {
				//
			}
			else {
				std::cout << str << " is not double" << ENTER;
				return false;
			}
			break;
		case Option::Type_::DATETIME:
			if (wiz::load_data::Utility::IsDate(str)) {
				//
			}
			else {
				std::cout << str << " is not date" << ENTER;
				return false;
			}
			break;
		case Option::Type_::DATETIME_A:
			if (wiz::load_data::Utility::IsDateTimeA(str)) {
				//
			}
			else {
				std::cout << str << " is not datetime A" << ENTER;
				return false;
			}
			break;
		case Option::Type_::DATETIME_B:
			if (wiz::load_data::Utility::IsDateTimeB(str)) {
				//
			}
			else {
				std::cout << str << " is not datetime B" << ENTER;
				return false;
			}
			break;
		case Option::Type_::QUOTED_STRING:
			if (str.size() >= 2 && str[0] == str.back() && str.back() == '\"') { // cf '\''
				//
			}
			else {
				std::cout << str << " is not quoted string" << ENTER;
				return false;
			}
			break;
		case Option::Type_::STRING:
			// pass
			break;
		}
		return true;
	}

	std::tuple<bool, Option, Option> _Check(wiz::load_data::UserType* schema_eventUT, 
		const wiz::load_data::ItemType<WIZ_STRING_TYPE>& x, const wiz::load_data::ItemType<WIZ_STRING_TYPE>& y) //, Order?
	{
		Option var_option = OptionFrom(x.GetName()); // name, value check - not start with % ??
		Option val_option = OptionFrom(x.Get(0));

		// val only case, ex) A = { a b c d } , a, b, c, d `s name is empty.
		if (x.GetName().empty()) {
			if (!y.GetName().empty()) {
				//
				return{ false, var_option, val_option };
			}
		}
		else { // bug?
			if (y.GetName().empty()) {
				//
				return{ false, var_option, val_option };
			}
		}
		
		// cf) optional && justOne -> error? or return { a, b, c} or add parameter?
		// required check. - for only var.? 
		// optional check. - for only var.?
		if (var_option.required != Option::Required_::REQUIRED
			&& var_option.optional == Option::Optional_::OPTIONAL_)
		{
			return{ true, var_option, val_option };
		}

		// option type check.
		if (OptionDoA(var_option, y.GetName()) &&
			OptionDoA(val_option, y.Get(0))) {

			// event check.
			wiz::ClauText clauText;
			wiz::StringTokenizer tokenizer(var_option.event_ids, &builder);
			wiz::StringTokenizer tokenizer2(val_option.event_ids, &builder);
			std::string event_name;
			
			while (tokenizer.hasMoreTokens()) {
				event_name = tokenizer.nextToken();

				// for var // chk no start with __, no end with __ ?
				wiz::load_data::LoadData::AddData(*schema_eventUT, "/./", 
					"Event = { id = __" + event_name + "__ $call = { id = " + event_name + 
					", name = " + y.GetName() + ", value = " + y.Get(0) + 
					", is_usertype = FALSE" + " } }",
					"TRUE", wiz::ExcuteData(), &builder);

				if ("TRUE" == clauText.excute_module("Main = { $call = { id = __" + event_name + "__ } }", schema_eventUT, wiz::ExcuteData(), opt, 0)) {
					schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);
				}
				else {
					schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);

					std::cout << "clauText is not valid" << ENTER;
					return { false, var_option, val_option };
				}
			}
			while (tokenizer2.hasMoreTokens()) {
				event_name = tokenizer2.nextToken();
				// for val
				wiz::load_data::LoadData::AddData(*schema_eventUT, "/./",
					"Event = { id = __" + event_name + "__ $call = { id = " + event_name +
					", name = " + y.GetName() + ", value = " + y.Get(0) +
					", is_usertype = FALSE" + " } }",
					"TRUE", wiz::ExcuteData(), &builder);

				if ("TRUE" == clauText.excute_module("Main = { $call = { id = __" + event_name + "__ } }", schema_eventUT, wiz::ExcuteData(), opt, 0)) {
					schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);
				}
				else {
					schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);

					std::cout << "clauText is not valid" << ENTER;
					return { false, var_option, val_option };
				}
			}
		}
		else {
			return { false, var_option, val_option };
		}

		return { true, var_option, val_option };
	}

	std::tuple<bool, Option> _Check(wiz::load_data::UserType* schema_eventUT,
		const wiz::load_data::UserType* x, const wiz::load_data::UserType* y // Order?
		) 
	{
		Option var_option = OptionFrom(x->GetName()); // name, value check - not start with % ??

		// val only case, ex) A = { a b c d } , a, b, c, d `s name is empty.
		if (x->GetName().empty()) {
			if (!y->GetName().empty()) {
				//
				return{ false, var_option };
			}
		}
		else { // bug?
			if (y->GetName().empty()) {
				//
				return{ false, var_option };
			}
		}

		// cf) optional && justOne -> error? or return { a, b, c} or add parameter?
		// required check. - for only var.? 
		// optional check. - for only var.?
		if (var_option.required != Option::Required_::REQUIRED
			&& var_option.optional == Option::Optional_::OPTIONAL_)
		{
			return{ true, var_option };
		}

		// option type check.
		if (OptionDoA(var_option, y->GetName())) {

			// event check.
			wiz::ClauText clauText;
			wiz::StringTokenizer tokenizer(var_option.event_ids, &builder);
			std::string event_name;

			while (tokenizer.hasMoreTokens()) {
				event_name = tokenizer.nextToken();

				// for var // chk no start with __, no end with __ ?
				wiz::load_data::LoadData::AddData(*schema_eventUT, "/./",
					"Event = { id = __" + event_name + "__ $call = { id = " + event_name +
					", name = " + y->GetName() +
					", is_usertype = TRUE } ",
					"TRUE", wiz::ExcuteData(), &builder);

				if ("TRUE" == clauText.excute_module("Main = { $call = { id = __" + event_name + "__ } }", schema_eventUT, wiz::ExcuteData(), opt, 0)) {
					schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);
				}
				else {
					schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);

					std::cout << "clauText is not valid" << ENTER;
					return { false, var_option };
				}
			}
		}
		else {
			return { false, var_option };
		}

		return { true, var_option };
	}

	bool Check(wiz::load_data::UserType* schema_eventUT, wiz::load_data::UserType* schemaUT, 
		const wiz::load_data::UserType* clautextUT)
	{
		Option::Order_ order = Option::Order_::OFF;
		long long ct_itCount = 0; // for clautextUT?
		long long ct_utCount = 0; // for clautextUT?
		long long itCount = 0;
		long long utCount = 0;
		
		for (long long i = 0; i < schemaUT->GetIListSize(); ++i) {
			if (schemaUT->IsItemList(i)) {
				if (schemaUT->GetItemList(itCount).ToString() == "%order_on") {
					order = Option::Order_::ON;
					continue;
				}
				else if (schemaUT->GetItemList(itCount).ToString() == "%order_off") {
					order = Option::Order_::OFF;
					continue;
				}

				// off -> no order? : slow??
				if (order == Option::Order_::OFF) {
					bool pass = false;
					long long count = 0;
					std::tuple<bool, Option, Option> temp;

					for (long long j = 0; j < clautextUT->GetItemListSize(); ++j) {
						if (std::get<0>(temp = _Check(schema_eventUT, schemaUT->GetItemList(itCount), clautextUT->GetItemList(j)))) {
							// justone check. - for no order?

							count++;
							pass = true;
						}
					}
					if (false == pass) {
						std::cout << "clauText is not valid" << ENTER;
						return false;
					}
					if (std::get<1>(temp).onemore == Option::OneMore_::JUSTONE) {
						if (count == 1) {

						}
						else {
							std::cout << "clauText is not valid" << ENTER;
							return false;
						}
					}
				}
				else if (order == Option::Order_::ON) {
					std::tuple<bool, Option, Option> temp;

					if (clautextUT->IsItemList(ct_itCount)) {
						if (std::get<0>(temp = _Check(schema_eventUT, schemaUT->GetItemList(itCount), clautextUT->GetItemList(ct_itCount)))) {
							//
						}
						else {
							std::cout << "clauText is not valid" << ENTER;
							return false;
						}
					}
					else {
						std::cout << "clauText is not valid" << ENTER;
						return false;
					}
				}
				ct_itCount++;
			}
			else { // usertype
				// off -> no order? : slow??
				if (order == Option::Order_::OFF) {
					bool pass = false;
					long long count = 0; // for just one?
					std::tuple<bool, Option> temp;

					for (long long j = 0; j < clautextUT->GetUserTypeListSize(); ++j) {
						if (std::get<0>(temp = _Check(schema_eventUT, schemaUT->GetUserTypeList(utCount), clautextUT->GetUserTypeList(j)))) {
							// justone check. - for no order?
							if (Check(schema_eventUT, schemaUT, clautextUT->GetUserTypeList(j))) {
								
							}
							else {
								std::cout << "clauText is not valid" << ENTER;
								return false;
							}
							count++;
							pass = true;
						}
					}
					if (false == pass) {
						std::cout << "clauText is not valid" << ENTER;
						return false;
					}
					if (std::get<1>(temp).onemore == Option::OneMore_::JUSTONE) {
						if (count == 1) {

						}
						else {
							std::cout << "clauText is not valid" << ENTER;
							return false;
						}
					}
				}
				else if (order == Option::Order_::ON) {
					std::tuple<bool, Option> temp;

					if (clautextUT->IsUserTypeList(ct_utCount)) {
						if (std::get<0>(temp = _Check(schema_eventUT, 
								schemaUT->GetUserTypeList(utCount), clautextUT->GetUserTypeList(ct_utCount)))) {
							if (Check(schema_eventUT, schemaUT, clautextUT->GetUserTypeList(ct_utCount))) {

							}
							else {
								std::cout << "clauText is not valid" << ENTER;
								return false;
							}
						}
						else {
							std::cout << "clauText is not valid" << ENTER;
							return false;
						}
					}
					else {
						std::cout << "clauText is not valid" << ENTER;
						return false;
					}
				}
				ct_utCount++;
			}
		}
		
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

		// __init__ first init.
		{
			wiz::ClauText clauText;
			clauText.excute_module("Main = { $call = { id = __init__ } }", &schema_eventUT, wiz::ExcuteData(), opt, 0);
		}

		//
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
		wiz::load_data::UserType ut;

		bool valid = wiz::load_data::LoadData::LoadDataFromFile3(fileName, ut, 0, 0);

		return { valid, std::move(ut) };
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


