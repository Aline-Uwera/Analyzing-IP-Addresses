#include "IPAddressAnalyze.h"
#include <algorithm>

#define MAX_IP_LENGTH 16					 
#define MAX_COUNT 100000					
#define MAX_LINE_LENGTH (MAX_IP_LENGTH + 10)

int IPAddressAnalyzer::readNextItemFromFile(FILE *inputFileStream)
{
	int item;
	if (fscanf(inputFileStream, "%d", &item) == 1)
	{
		return item;
	}
	else
	{
		if (feof(inputFileStream))
		{
			throw std::ios_base::failure("End of file reached.");
		}
		else
		{
			fseek(inputFileStream, 0, SEEK_END);
			clearerr(inputFileStream);
			return readNextItemFromFile(inputFileStream);
		}
	}
}


void IPAddressAnalyzer::getMostFrequentIPAddress(char *inputFilePath, char *outputFilePath, int n)
{
	FILE *inFileStream = fopen(inputFilePath, "r");
	if (!inFileStream)
	{
		char message[1024];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
	}

	FILE *outFileStream = fopen(outputFilePath, "w");
	if (!outFileStream)
	{
		char message[1024];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
	}

	// Data structures to store IP addresses and their counts
	struct IPAddressCount
	{
		char ipAddress[MAX_IP_LENGTH];
		int count;
	};

	IPAddressCount ipCounts[MAX_COUNT];
	int ipCount = 0;

	// Read IP addresses from the input file and update counts
	while (true)
	{
		char ipString[MAX_IP_LENGTH];
		int count;
		if (fscanf(inFileStream, "%[^,], %d\n", ipString, &count) == 2)
		{
			// Check if IP address already exists in the array
			bool found = false;
			for (int i = 0; i < ipCount; ++i)
			{
				if (strcmp(ipCounts[i].ipAddress, ipString) == 0)
				{
					ipCounts[i].count += count;
					found = true;
					break;
				}
			}

			if (!found)
			{
				strcpy(ipCounts[ipCount].ipAddress, ipString);
				ipCounts[ipCount].count = count;
				ipCount++;
			}
		}
		else
		{
			// End of file reached
			break;
		}
	}

	// Sort IP addresses based on counts in descending order
	std::sort(ipCounts, ipCounts + ipCount, [](const IPAddressCount &a, const IPAddressCount &b)
			  { return a.count > b.count; });

	// Write top "n" IP addresses to output file
	int rank = 1;
	int prevCount = -1;
	for (int i = 0; i < n && i < ipCount; ++i)
	{
		if (ipCounts[i].count != prevCount)
		{
			rank = i + 1;
		}
		int j = i;
		while (j < ipCount && ipCounts[j].count == ipCounts[i].count)
		{
			fprintf(outFileStream, "%d, %s, %d\n", rank, ipCounts[j].ipAddress, ipCounts[j].count);
			++j;
		}
		prevCount = ipCounts[i].count;
		i = j - 1; // Update i to the last index processed
	}

	fflush(outFileStream);
	fclose(outFileStream);
	fclose(inFileStream);
}


// void IPAddressAnalyzer::getMostFrequentIPAddress(char *inputFilePath, char *outputFilePath, int n)
// {
// 	FILE *inFileStream = fopen(inputFilePath, "r");
// 	if (!inFileStream)
// 	{
// 		char message[1024];
// 		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
// 		throw std::ios_base::failure(message);
// 	}

// 	FILE *outFileStream = fopen(outputFilePath, "w");
// 	if (!outFileStream)
// 	{
// 		char message[1024];
// 		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
// 		throw std::ios_base::failure(message);
// 	}

// 	// Data structures to store IP addresses and their counts
// 	struct IPAddressCount
// 	{
// 		char ipAddress[MAX_IP_LENGTH];
// 		int count;
// 	};

// 	IPAddressCount ipCounts[MAX_COUNT];
// 	int ipCount = 0;

// 	// Read IP addresses from the input file and update counts
// 	while (true)
// 	{
// 		char ipString[MAX_IP_LENGTH];
// 		int count;
// 		if (fscanf(inFileStream, "%[^,], %d\n", ipString, &count) == 2)
// 		{
// 			// Check if IP address already exists in the array
// 			bool found = false;
// 			for (int i = 0; i < ipCount; ++i)
// 			{
// 				if (strcmp(ipCounts[i].ipAddress, ipString) == 0)
// 				{
// 					ipCounts[i].count += count;
// 					found = true;
// 					break;
// 				}
// 			}

// 			if (!found)
// 			{
// 				strcpy(ipCounts[ipCount].ipAddress, ipString);
// 				ipCounts[ipCount].count = count;
// 				ipCount++;
// 			}
// 		}
// 		else
// 		{
// 			// End of file reached
// 			break;
// 		}
// 	}

// 	// Sort IP addresses based on counts in descending order
// 	for (int i = 0; i < ipCount - 1; ++i)
// 	{
// 		for (int j = 0; j < ipCount - i - 1; ++j)
// 		{
// 			// Sort by count in descending order
// 			if (ipCounts[j].count < ipCounts[j + 1].count)
// 			{
// 				// Swap IP addresses and counts
// 				IPAddressCount temp = ipCounts[j];
// 				ipCounts[j] = ipCounts[j + 1];
// 				ipCounts[j + 1] = temp;
// 			}
// 			else if (ipCounts[j].count == ipCounts[j + 1].count)
// 			{
// 				// If counts are equal, compare IP addresses
// 				if (strcmp(ipCounts[j].ipAddress, ipCounts[j + 1].ipAddress) > 0)
// 				{
// 					// Swap IP addresses and counts
// 					IPAddressCount temp = ipCounts[j];
// 					ipCounts[j] = ipCounts[j + 1];
// 					ipCounts[j + 1] = temp;
// 				}
// 			}
// 		}
// 	}

// 	// Write top "n" IP addresses to output file
// 	int rank = 1;
// 	int prevCount = -1;
// 	for (int i = 0; i < n && i < ipCount; ++i)
// 	{
// 		if (ipCounts[i].count != prevCount)
// 		{
// 			rank = i + 1;
// 		}
// 		int j = i;
// 		while (j < ipCount && ipCounts[j].count == ipCounts[i].count)
// 		{
// 			fprintf(outFileStream, "%d, %s, %d\n", rank, ipCounts[j].ipAddress, ipCounts[j].count);
// 			++j;
// 		}
// 		prevCount = ipCounts[i].count;
// 		i = j - 1; // Update i to the last index processed
// 	}

// 	fflush(outFileStream);
// 	fclose(outFileStream);
// 	fclose(inFileStream);
// }
