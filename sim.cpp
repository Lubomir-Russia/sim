#include <iostream>
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
    if (argc < 1) {
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
}
