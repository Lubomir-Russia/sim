#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator

using namespace std;

static void show_usage(string name)
{
    cerr << name << " is a simple simulator of ENNA's partner systems.\n"
         << "Usage: " << name << " <option> ALARM_IDs\n"
         << "Options:\n"
         << "\t-h,--help\t\tShow this help message\n"
         << "\t-a,--add\t\tAdd alarms with ALARM_IDs\n"
         << "\t-d,--delete\t\tDelete alarms with ALARM_IDs\n"
         << "Example:\n" << name << " -a My_alarm_A01 My_alarm_A02 AL2013-11-01\n"
         << endl;
}

string getEnvVar(string const & key)
{
    char * val = getenv( key.c_str() );
    return val == NULL ? string("") : string(val);
}

string getTimeStamp() {

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char arcString [32];
    string time_stamp;

    if (strftime (&(arcString [0]), 20, "%Y-%m-%d_%H-%M-%S", timeinfo) != 0)
    {
        return time_stamp = (char*) &(arcString [0]);
    }
    else
    {
        return time_stamp = "1970-01-01_00-00-00";
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        show_usage(argv[0]);
        return 1;
    }
    vector <string> alarms;
    string action;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "-a") || (arg == "--add")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                action = "add";
                for (int j = i+1; j < argc; ++j) {
                    alarms.push_back(argv[j]);
                }
                break;
            } else {
                cerr << "--add option requires at least one alarm_id." << endl;
                return 1;
            }
        } else if ((arg == "-d") || (arg == "--delete")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                action = "delete";
                for (int j = i+1; j < argc; ++j) {
                    alarms.push_back(argv[j]);
                }
                break;
            } else {
                cerr << "--delete option requires at least one alarm_id." << endl;
                return 1;
            }
        } else {
            cerr << "Option " << argv[i] << " is unknown. Use -h for usage." << endl;
        }
    }

    if (action == "add") {
        cout << "Action:\t\tadd" << endl;
    }
    if (action == "delete") {
        cout << "Action:\t\tdelete" << endl;
    }

    string user_name = getlogin();
    cout << "User name:\t" << user_name << endl;

    cout << "Alarms:\t\t";
    copy(alarms.begin(), alarms.end(), ostream_iterator<string>(cout, ", "));
    cout << endl;

    string time_stamp = getTimeStamp();
    string report_name = "report_" + time_stamp + "_" + user_name + ".html";
    cout << "Report name:\t" << report_name << endl;

    string enna_path = getEnvVar("ENNA_PATH");
    if (enna_path == "") {
        cout << "Env variable $ENNA_PATH is not set. Current Working dir will be used used" << endl;
    }
    else {
        enna_path +="/";
        cout << "ENNA path:\t" << enna_path << endl;
    }

    string report_full_name = enna_path + report_name;
    cout << "Report full name:\t" << report_full_name << endl;

    string page = "Ok";
    if (action == "add") {
        ofstream myfile;
        myfile.open (report_full_name.c_str());
        myfile << "<!DOCTYPE html>\n<html>\n<head></head>\n<body>\n"; //starting html

        //add some html content
        //as an example: if you have array of objects featuring the properties name & value, you can print out a new line for each property pairs like this:
        // for (int i=0; i< alarms.length(); i++)
        for (vector<string>::iterator i = alarms.begin(); i != alarms.end(); ++i)
            myfile << "\n\t<p>Alarm class: <b>" << *i << "</b> "
                   << "imported by <i>" << user_name << "</i> "
                   << "at " << time_stamp ;

        //ending html
        myfile << "\n</body>\n</html>";
        myfile.close();
        string name = getlogin();
        cout << endl << page << endl;
    }
}
