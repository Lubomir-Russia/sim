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
        cout << "Action is add" << endl;
    }
    if (action == "delete") {
        cout << "Action is delete" << endl;
    }
    copy(alarms.begin(), alarms.end(), ostream_iterator<string>(cout, " "));
    string page = "Ok";
    string report_name = "report";
    string user_name = getlogin();
    cout << "User name is: " << user_name << endl;
    char arcString [32];
    string strTmp;

    // add start-date/start-time

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    if (strftime (&(arcString [0]), 20, "%Y-%m-%d_%H-%M-%S", timeinfo) != 0)
    {
        strTmp = (char*) &(arcString [0]);
    }
    else
    {
        strTmp = "1970-01-01_00-00-00";
    }
    report_name += "_" + strTmp + "_" + user_name + ".html";
    cout << report_name << endl;
    if (action == "add") {
        ofstream myfile;
        myfile.open (report_name.c_str());
        myfile << "<!DOCTYPE html><html><head></head><body>"; //starting html

        //add some html content
        //as an example: if you have array of objects featuring the properties name & value, you can print out a new line for each property pairs like this:
        // for (int i=0; i< alarms.length(); i++)
        for (vector<string>::iterator i = alarms.begin(); i != alarms.end(); ++i)
            myfile << "<p><span style='font-weight: bold'>" << *i << "</span><span>" << "10019" << "</span></p>";

        //ending html
        myfile << "</body></html>";
        myfile.close();
        string name = getlogin();
        cout << endl << name << endl << page;
    }
}
