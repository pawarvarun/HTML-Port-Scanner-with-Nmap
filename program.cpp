#include <iostream>
#include <fstream>
using namespace std;
int found;
string data_port[100];
string data_state[100];
string data_service[100];
string data_version[100];
string data_tip[100];

struct
{
	int init_pos;
	int final_pos;
	string data;
}

port, state, service, version;

void asciiart(){
	
	cout<< R"(

__________              __      _________                                         
\______   \____________/  |_   /   _____/ ____ _____    ____   ____   ___________ 
 |     ___/  _ \_  __ \   __\  \_____  \_/ ___\\__  \  /    \ /    \_/ __ \_  __ \
 |    |  (  <_> )  | \/|  |    /        \  \___ / __ \|   |  \   |  \  ___/|  | \/
 |____|   \____/|__|   |__|   /_______  /\___  >____  /___|  /___|  /\___  >__|   
                                      \/     \/     \/     \/     \/     \/       

)";
}
bool isNmapInstalled()
{
	FILE *pipe = popen("which nmap", "r");
	if (pipe)
	{
		char buffer[128];
		string result = "";
		while (!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != nullptr)
				result += buffer;
		}

		pclose(pipe);
		return !result.empty();
	}

	return false;
}

void command()
{
	if (!isNmapInstalled())
	{
		cout << "nmap is not installed on your system." << endl;
	}

	string hostname;
	cout << "Enter hostname or IP: ";
	cin >> hostname;
	cout<<"\n\nScanning Target...\n";

	string command = "nmap -sV " + hostname + " > /tmp/tempoutput.txt";
	system(command.c_str());
	
	cout<<"\nTarget Scanned!\nGenerating Output file.\n";
}

//DO IT
string rtrim(const std::string &s)
{
	size_t end = s.find_last_not_of(" ");
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}

//function to remove extra whitespace at the end
string space_remover(string input1)
{
	string input = input1;
	while (0)
	{
		int len = input.length() - 1;
		if (input[len] == ' ')
		{
			input = input.substr(0, len - 1);
		}
		else
		{
			break;
		}
	}
	return input;
}

//function to reset values
void reset()
{
	port.data = "";
	state.data = "";
	service.data = "";
	version.data = "";
}

int fin(string word, string str)
{
	//find particular word in the given string. Syntax: fin(word to find, string in which its to be find);
	found = str.find(word);
	if (found != std::string::npos)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//function to find space indexes
int space(string line)
{
	int len = line.size();
	int count2 = 0;
	port.init_pos = 0;
	for (int i = 0; i < len; i++)
	{
		if (count2 == 0)	//for finding port index
		{
			if (line.at(i) == 'S')
			{
				state.init_pos = i;
				port.final_pos = i - 1; 
				count2++;
				i++;
			}
		}

		if (count2 == 1)
		{
			//for finding state index
			if (line.at(i) == 'S')
			{
				service.init_pos = i;
				state.final_pos = i - 1;
				count2++;
				i++;
			}
		}

		if (count2 == 2)
		{
			//for finding service index &version index
			if (line.at(i) == 'V' && line.at(i + 1) == 'E')
			{
				service.final_pos = i - 1;
				version.init_pos = i;
				version.final_pos = len;
				count2++;
			}
		}
	}
}

void helper(int i)
{
	if (data_service[i] == "ftp")
	{
		data_tip[i] = R"(
        1) Check for anonymous login
        <br>2) brute forcing passwords
        <br>3) Directory traversal attacks)";
	}
	else if (data_service[i] == "ssh")
	{
		data_tip[i] = R"(
        <br>1) Leaked ssh keys
        <br>2) brute forcing credential details)";
	}
	else if (data_service[i] == "http" || data_service[i] == "ssl/http")
	{
		data_tip[i] = R"(
        1) Cross-site Scripting
        <br>2) SQL Injections
        <br>3) Gobuster
        <br>4) Fuzzing
        <br>5) Viewing page Source
        <br>6) Modifying Requests)";
	}
	else if (data_service[i] == "telnet")
	{
		data_tip[i] = R"(
            1) Session Hijacking
            <br>2) Remote Code Execution
            <br>3) Man-in-the-Middle (MITM))";
	}
	else if (data_service[i] == "smb")
	{
		data_tip[i] = R"(
            1) Capturing NTLM hashes
            <br>2) Brute-forcing SMB login credentials
            <br>3) Anonymous login in smbclient)";
	}
	else	//if (data_service[i] == "unknown") 
	{
		data_tip[i] = "Try Googling about it.";
	}

	//cout<<i<<". "<<data_tip[i]<<endl<<data_service[i]<<"."<<endl<<endl;

}

 int outputfile() {
    int i;
    FILE *file;
    file = fopen("report.txt", "w");
    
    if (file == NULL) {
        printf("Unable to create the file.\n");
        return 1;
    }
       ofstream htmlFile("Network_report.html");
    if (!htmlFile) {
        cout << "Failed to create HTML file." << endl;
        return 1;
    }
int n=100;
    htmlFile << R"(<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Network Report</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous">
  </head>
  <style type="text/css">
      body {
        background-color: #000000;
        color: #fff;
        padding: 1em;
      }
      .table-cust {
        width: 90%;
        margin-right: auto;
        margin-left: auto;
      }
  </style>
  <body>
    <h1><center>Port Scanner</center></h1>
    <div class="table-responsive-md">
    <table class="table table-dark table-striped table-cust">
        <thead>
            <tr>
                <th>Port</th>
                <th>State</th>
                <th>Service</th>
                <th>Version</th>
            </tr>
      </thead>
      <tbody>)";

    for (i=0;i<n;i++){
    //htmlFile<< R"(<td>)"<<data_port[i],data_state[i],data_service[i],data_version[i]<<R"(</td)";
    if(data_port[i]!=""){
    htmlFile<< R"(
		<tr>
        <td>)"<<data_port[i]<< R"(</td>
    <td>)"<<data_state[i]<< R"(</td>
    <td>)"<<data_service[i]<< R"(</td>
    <td>)"<<data_version[i]<< R"(</td>
    </tr>)";
    }
	else{
		break;
	}

}

htmlFile << R"(</tbody>
    </table>
    </div>

    <div class="table-responsive-md">
    <table class="table table-dark table-striped table-cust table-bordered border-light">
        <tbody>)";

for(int i=0;i<=100;i++){
    if(data_port[i]!=""){
    htmlFile << R"(
        <tr><td>)"<<data_service[i]<<R"(</td>
        <td>)"<<data_tip[i]<<R"(</td>
		<td>)";
		i++;
		htmlFile << data_service[i]<<R"(</td>
        <td>)"<<data_tip[i]<<R"(</td></tr>)";
		
}
else{
	break;
}
}


htmlFile << R"(</tbody>
    </table>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4" crossorigin="anonymous"></script>
  </body>
</html>)";


    htmlFile.close();
    cout << "HTML file created successfully." << endl;

    // Open the file with the default program
   // string openCommand = "xdg-open cool_table.html";  // Modify this command based on your system
   // system(openCommand.c_str());

    return 0;

    
    fclose(file); 
}

void data_manage(){
	ifstream inputfile("/tmp/tempoutput.txt");
	string line;

	int count = 0;
	string str = "";
	int i = 0;
	while (getline(inputfile, line))
	{
		if (fin("PORT", line))
		{
			count++;
			space(line);

			//uncomment this to print word indexes
			//cout<<port.init_pos<<endl<<port.final_pos<<endl<<state.init_pos<<endl<<state.final_pos<<endl<<service.init_pos<<endl<<service.final_pos<<endl<<version.init_pos<<endl<<version.final_pos;
		}
		else if (fin("incorrect", line) || fin("Info", line))
		{
			//used to break once port details part completed
			count = 0;
		}
		else if (count == 1)
		{
			if (!line.empty())
			{
			 	//uncomment this to print line
				//cout << line << endl;
				int count2 = 0;
				int len = line.size();

				for (int j = 0; j < len; j++)
				{
					if (j >= port.init_pos && j <= port.final_pos)
					{
						port.data = port.data + (line.at(j));
						port.data = space_remover(port.data);
					}

					if (j >= state.init_pos && j <= state.final_pos)
					{
						state.data = state.data + (line.at(j));
						state.data = space_remover(state.data);
					}

					if (j >= service.init_pos && j <= service.final_pos)
					{
						service.data = service.data + (line.at(j));
						service.data = space_remover(service.data);
					}

					if (j >= version.init_pos && j <= len)
					{
						version.data = version.data + (line.at(j));
						version.data = space_remover(version.data);
					}
				}

				data_port[i] = rtrim(port.data);
				data_state[i] = rtrim(state.data);
				data_service[i] = rtrim(service.data);
				data_version[i] = rtrim(version.data);

				reset();
				helper(i);
				//cout << i << ". " << data_port[i] << endl << data_state[i] << endl << data_service[i] << endl << data_version[i] << endl << data_tip[i] << endl << endl;
				i++;
			}
		}
	}
}

int main()
{
	asciiart();
	command();
	data_manage();
    outputfile();
}
