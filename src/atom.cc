#include "atom.h"
#include "ctemplate/template_enums.h"

#include <format>
#include <iostream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>

#include <ctemplate/template.h>

namespace yass {

using ctemplate::ExpandTemplate;;
using ctemplate::StringToTemplateCache;
using ctemplate::TemplateDictionary;

constexpr char kTemplateString[] = R"atom(
<?xml version="1.0" encoding="UTF-8"?>
<feed xmlns="http://www.w3.org/2005/Atom">
	<title>{{FEED_NAME}}</title>
	<id>{{HOMEPAGE_URL}}</id>
	<link rel="alternate" href="{{HOMEPAGE_URL}}"/>
	<link href="{{FEED_URL}}" rel="self"/>
	<updated>{{LAST_UPDATE_TIME}}</updated>
	<author>
		<name>{{AUTHOR_NAME}}</name>
	</author>
	{{#ENTRY}}
	<entry>
		<title>{{TITLE}}</title>
		<link rel="alternate" type="text/html" href="{{HTML_URL}}"/>
		<id>{{HTML_URL}}</id>
		<published>{{PUBLISHED_TIME}}</published>
		<updated>{{UPDATE_TIME}}</updated>
		<content type="html"><![CDATA[{{CONTENT}}]]></content>
	</entry>
	{{/ENTRY}}
</feed>
)atom";

constexpr char kDateFormat[] = "{0:%F}T{0:%T%Ez}";

namespace {
std::string GetTimeString(time_t &time) {
    return std::format(kDateFormat,
            std::chrono::floor<std::chrono::seconds>(
                std::chrono::system_clock::from_time_t(time)));
}
} // namesapce

Atom::Atom(const SiteConfig& site_config) :
       name_(site_config.name),
       homepage_url_(site_config.base_url),
       feed_url_(site_config.base_url + "feed.xml"),
       author_(site_config.copyright_owner),
       update_time_(std::time(0)),
       entries_()
    {
        StringToTemplateCache(
                "atom", kTemplateString, ctemplate::STRIP_WHITESPACE);
    }
    
void Atom::AddEntry(const Entry& entry) {
    entries_.push_back(entry);
}

void Atom::Write(std::filesystem::path out_dir) {
	TemplateDictionary dict("atom");
	dict.SetValue("FEED_NAME", name_);
	dict.SetValue("HOMEPAGE_URL", homepage_url_);
	dict.SetValue("FEED_URL", feed_url_);
	dict.SetValue("AUTHOR_NAME", author_);
    dict.SetValue("LAST_UPDATE_TIME", GetTimeString(update_time_));

	for (auto &entry: entries_) {
		TemplateDictionary *entry_dict = dict.AddSectionDictionary("ENTRY");
		entry_dict->SetValue("TITLE", entry.title);
		entry_dict->SetValue("HTML_URL", homepage_url_ + entry.url);
		entry_dict->SetValue("CONTENT", entry.content);
        entry_dict->SetValue("PUBLISHED_TIME", GetTimeString(entry.published_time));
		entry_dict->SetValue("UPDATE_TIME", GetTimeString(entry.update_time));
	}		
	std::string output;
	ExpandTemplate("atom", ctemplate::STRIP_WHITESPACE, &dict,
		 &output);
    std::ofstream out(out_dir/"feed.xml");
	out << output;
}

} /* yass  */ 
