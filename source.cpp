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

void command()
{
	if (!isNmapInstalled())
	{
		cout << "nmap is not installed on your system." << endl;
		exit(0);
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
	int end = s.find_last_not_of(" ");
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}

//function to remove extra whitespace at the end


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
						//port.data = space_remover(port.data);
					}

					if (j >= state.init_pos && j <= state.final_pos)
					{
						state.data = state.data + (line.at(j));
						//state.data = space_remover(state.data);
					}

					if (j >= service.init_pos && j <= service.final_pos)
					{
						service.data = service.data + (line.at(j));
						//service.data = space_remover(service.data);
					}

					if (j >= version.init_pos && j <= len)
					{
						version.data = version.data + (line.at(j));
						//version.data = space_remover(version.data);
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
