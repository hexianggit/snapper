/*
 * Copyright (c) [2019] SUSE LLC
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */

#include <iostream>

#include "client/utils/text.h"
#include "client/Command/ListConfigs.h"
#include "client/Command/ListConfigs/Options.h"
#include "client/Command/ListConfigs/SnappersData/Table.h"
#include "client/Command/ListConfigs/SnappersData/Csv.h"
#include "client/Command/ListConfigs/SnappersData/Json.h"

using namespace std;

namespace snapper
{
    namespace cli
    {

	string Command::ListConfigs::help()
	{
	    return
		string(
		    _("  List configs:\n"
	 	      "\tsnapper list-configs\n"
		      "\n")
		) + Options::help_text();
	}


	Command::ListConfigs::ListConfigs(const GlobalOptions& global_options,
	    GetOpts& options_parser, ProxySnappers& snappers) :
	    Command(global_options, options_parser, snappers), _options(new Options(options_parser))
	{}


	Command::ListConfigs::ListConfigs::~ListConfigs()
	{}


	const Command::ListConfigs::Options& Command::ListConfigs::options() const
	{
	    return *_options.get();
	}


	vector<string> Command::ListConfigs::errors() const
	{
	    return options().errors();
	}


	void Command::ListConfigs::run()
	{
	    if (_options_parser.hasArgs())
	    {
		cerr << _("Command 'list-configs' does not take arguments.") << endl;
		exit(EXIT_FAILURE);
	    }

	    if (options().has_errors())
	    {
		cerr << options().errors().front() << endl;
		exit(EXIT_FAILURE);
	    }

	    string output;

	    switch (global_options().output_format())
	    {
		case GlobalOptions::OutputFormat::TABLE:
		{
		    SnappersData::Table table(*this, global_options().table_style());
		    output = table.output();
		}
		break;

		case GlobalOptions::OutputFormat::CSV:
		{
		    SnappersData::Csv csv(*this, global_options().separator());
		    output = csv.output();
		}
		break;

		case GlobalOptions::OutputFormat::JSON:
		{
		    SnappersData::Json json(*this);
		    output = json.output() + "\n";
		}
		break;
	    }

	    cout << output;
	}

    }
}
