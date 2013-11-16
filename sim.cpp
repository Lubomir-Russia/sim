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

string getTimeStamp()
{

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

string getEnvVar(string const & key)
{
    char * val = getenv( key.c_str() );
    return val == NULL ? string("") : string(val);
}

string getAbsPath(string const & key)
{
    string path = getEnvVar(key);
    if (path == "") {
        cout << "Env variable $" << key << " is not set. Current Working dir will be used used" << endl;
    }
    else {
        path +="/";
        cout << key << " path:\t" << path << endl;
    }
    return path;
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

    string enna_path = getAbsPath("ENNA_PATH");
    string report_path = getAbsPath("ENNA_REPORTS");

    string report_full_name = report_path + report_name;
    cout << "Report full name:\t" << report_full_name << endl;

    string page = "Ok";
    if (action == "add") {
        // create alarm files
        ofstream myfile;
        srand ( time(NULL) );
        string help_abs_dir = getEnvVar("ENNA_HELP");
        string help_rel_dir = help_abs_dir.substr(help_abs_dir.find_last_of("\\/"));
        for (vector<string>::iterator i = alarms.begin(); i != alarms.end(); ++i) {
            int image_number = rand() % 22 + 1;
            // string image_file = str( format("Alarm_%1\.jpg") % image_number;
            char buff[100];
            sprintf(buff, "Alarm_%02d.jpg", image_number);
            string image_file = buff;
            string image_rel_path = help_rel_dir + "\\" + image_file;
            string alarm_full_name = enna_path + "alarm_" +*i + ".html";
            myfile.open (alarm_full_name.c_str());
            myfile << "<!DOCTYPE html>\n<html>\n<head></head>\n<body>\n"; //starting html
            myfile << "\n\t<font size=\"7\" color=\"#ff0033\">Alarm class: <b>" << *i << "</b></font><p>"
                   << "\n\t<p>Generated at " << time_stamp
                   << "\n\t<p>Upploaded by <i>" << user_name << "</i>"
                   << "\n\t<p><p>"
                   << "\n\t<img src=\"" << image_rel_path << "\" width=\"180\" height=\"160\" border=\"0\" alt=\"\">"
                   << "\n\t<p>" 
                   << "\n\t <hr><font size=\"5\" color=\"#990000\">" 
                   << "\n\t<script>"
                   << "\n\t  document.write('<a href=\"' + document.referrer + '\">Go back.</a>');" 
                   << "\n\t</script>"
                   << "\n\t<a href=\"/\">Main Page</a></font>"
                   ;
            //ending html
            myfile << "\n</body>\n</html>";
            myfile.close();
        }


        // create report file
        myfile.open (report_full_name.c_str());
        myfile << "<!DOCTYPE html>\n<html>\n<head></head>\n<body>\n"; //starting html

        //add some html content
        //as an example: if you have array of objects featuring the properties name & value, you can print out a new line for each property pairs like this:
        // for (int i=0; i< alarms.length(); i++)
        for (vector<string>::iterator i = alarms.begin(); i != alarms.end(); ++i)
            myfile << "\n\t<p>Alarm class: <b>" << *i << "</b> "
                   << "imported by <i>" << user_name << "</i> "
                   << "at " << time_stamp ;

        myfile  << "\n\t<p>" 
                << "\n\t <hr><font size=\"5\" color=\"#990000\">" 
                << "\n\t<script>"
                << "\n\t  document.write('<a href=\"' + document.referrer + '\">Go back.</a>');" 
                << "\n\t</script>"
                << "\n\t<a href=\"/\">Main Page</a></font>"
                ;
        //ending html
        myfile << "\n</body>\n</html>";
        myfile.close();
        string name = getlogin();
        cout << endl << page << endl;
    }
}
