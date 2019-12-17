#include "pch.h"
#include "Command.h"
using namespace std;

#define BUF_STD_SIZE 256

string FileHandler::SendFile(const char* name)
{
    // File to read
    FILE* fin;
    string buf;

    if (!fopen_s(&fin, name, "rb")) {

        // The end of the file
        fseek(fin, 0, SEEK_END);
        // Get size of the file
        unsigned int m_file_size = ftell(fin);
        cout << m_file_size << endl;
        // Go to start
        rewind(fin);

        unsigned int BLOCK_SIZE;
        size_t CountOfRead = 0;
        // Read and write by "BLOCK_SIZE" bytes
        for (unsigned int i = 0; i < m_file_size; )
        {
            if (BUF_STD_SIZE > m_file_size)
                BLOCK_SIZE = m_file_size;
            else
                BLOCK_SIZE = BUF_STD_SIZE;

            char* buffer = new char[BLOCK_SIZE];

            // Read "BLOCK_SIZE" bytes to buffer
            CountOfRead = fread(buffer, sizeof(unsigned char), BLOCK_SIZE, fin);
            i += CountOfRead;
            // Write "BLOCK_SIZE" bytes
            buf.append(buffer, CountOfRead);
         
            delete buffer;
        }

		fclose(fin);
		_error_report = "Succesfully read file";
        return buf;
    }
	else
	{
		_error_report = "Failed with reading the file";
		buf.clear();
		return buf;
	}
}

string FileHandler::RecieveFile(string fileReadBuf, const char* name)
{
    // File to write
    FILE* fout;
    string result;
    int countOfCycles = 0;

    if (!fopen_s(&fout, name, "wb"))
    {
        unsigned int BLOCK_SIZE;
        size_t CountOfWrite = 0;
        unsigned int fileSize = fileReadBuf.size();

        // Write by "BLOCK_SIZE" bytes
        for (unsigned int i = 0; i < fileSize; )
        {
            if (BUF_STD_SIZE > fileReadBuf.size() - i)
                BLOCK_SIZE = fileReadBuf.size() - i;
            else
                BLOCK_SIZE = BUF_STD_SIZE;

            CountOfWrite = fwrite(fileReadBuf.substr(countOfCycles * BUF_STD_SIZE, BLOCK_SIZE).c_str(), sizeof(unsigned char), BLOCK_SIZE, fout);

            i += CountOfWrite;
            ++countOfCycles;
        }
        fclose(fout);
        result = "0";
		_error_report = "Succesfully writting file";

    }
	else
	{
		result = "-1";
		_error_report = "Failed with writting the file";
	}
    return result;
}