#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//* Macros for the alphabet *//
#define ALPHABET_SIZE 26
#define ALPHABET_CHARACTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
const float ALPHABET_PROBABILITIES[ALPHABET_SIZE] = {0.1463, 0.0104, 0.0388, 0.0499, 0.1257,
		   0.0102, 0.013, 0.0128, 0.0618, 0.004, 0.0002, 0.0278, 0.0474, 0.0505, 0.173, 0.0252,
		   0.012, 0.0653, 0.0781, 0.0434, 0.0463, 0.0167, 0.0001, 0.0021, 0.0001, 0.0047
};

//* AUXILIARY *//
void toupper_case(char* str, char* new_str)
{
	for(int i = 0; str[i] != '\0'; i++)
	new_str[i] = toupper(str[i]);
}

void calculate_frequencies(char* str, float* frequencies)
{
	for(int i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ' ')
			continue;
			
		// if the character is not a space, find its index in the ALPHABET_CHARACTERS array and increment the corresponding frequency
		for(int j = 0; j < ALPHABET_SIZE; j++)
			if(str[i] == ALPHABET_CHARACTERS[j])
			{
				frequencies[j]++;
				break;
			}
	}
}

void calculate_probabilities(char* str, float* probabilities)
{
	// Calculate the frequencies to auxilate the probabilities
	float frequencies[ALPHABET_SIZE] = {0};
	calculate_frequencies(str, frequencies);

	// Calculate the probabilities
	for(int i = 0; i < ALPHABET_SIZE; i++)
		probabilities[i] = frequencies[i] / strlen(str);
}
//* END OF AUXILIARY *//

//* CAESAR CIPHER *//
/**
 * @brief encrypts a string using the Caesar cipher
 * 
 * @param X string to be encrypted
 * @param Y string to store the encrypted string
 * @param k key for the encryption
 */
void caesar_encrypt(char* X, char* Y, int k)
{
	toupper_case(X, Y);
	
	// Loop through each character in the string and apply the Cesar cipher to all that are not blank spaces
	for(int i = 0; Y[i] != '\0'; i++)
		if(Y[i] != ' ')
			Y[i] = (Y[i] - 'A' + k) % 26 + 'A';
}

/**
 * @brief decrypts a string using the Caesar cipher
 * 
 * @param Y string to be decrypted
 * @param X string to store the decrypted string
 * @param k key for the decryption
 */
void caesar_decrypt(char* Y, char* X, int k)
{
	toupper_case(Y, X);
	
	// Loop through each character in the string and apply the Cesar cipher to all that are not blank spaces
	for(int i = 0; X[i] != '\0'; i++)
		if(X[i] != ' ')
			X[i] = (X[i] + 'A' - k) % 26 + 'A';
}
//* END OF CAESAR CIPHER *//

//* CYPTOANALYSIS *//
/**
 * @brief try to find the key used in the encryption to decrypt the string
 * 
 * @param Y string to be decrypted
 */
void crypto_analysis(char* Y)
{
	// Initialize arrays to store frequencies and probabilities of the letters found in the string
	float probabilities[ALPHABET_SIZE] = {0};

	// Initialize the string to store the decrypted string
	char X[256];
	
	// Calculate the probabilities
	calculate_probabilities(Y, probabilities);

	// Find the key by comparing the calculated probabilities with the known probabilities
	int k = 0;
	int max_prob_index = 0;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if(probabilities[i] > probabilities[max_prob_index])
			max_prob_index = i;
	// considers 'A' as the most frequent letter in the alphabet
	k = (max_prob_index + ALPHABET_SIZE) % ALPHABET_SIZE;

	// Decrypt the string
	caesar_decrypt(Y, X, k);

	// Print the decrypted string
	printf("Decrypted by cryptanalysis string: %s\n", X);
	printf("Key based on probability: %d\n", k);


}
//* END OF CRYPTOANALYSIS *//


//* MAIN FUNCTION *//
int main()
{
	// Allocate memory for strings
	char* X = (char*)malloc(256 * sizeof(char));
	char* Y = (char*)malloc(256 * sizeof(char));
	char* Z = (char*)malloc(256 * sizeof(char));
	int k;
	
	printf("*********************************************************\n");
	printf("**********************CAESAR CIPHER**********************\n");
	printf("*********************************************************\n");

	// Prompt user for input string
	printf("Enter the string to encrypt: ");
	fgets(X, 256, stdin);        // function to read a string from stdin(keyboard) and put inside X, which has a size of 256 bytes
	X[strcspn(X, "\n")] = '\0';  // Remove newline character from input string
	
	// Prompt user for key
	printf("Enter the key: ");
	scanf("%d", &k);
	printf("\n");
	

	// Encrypt the string
	printf("*********************************************************\n");
	printf("**********************ENCRYPTION*************************\n");
	printf("*********************************************************\n\n");
	caesar_encrypt(X, Y, k);
	printf("Encrypted string: %s\n\n", Y);


	// Cryptoanalysis
	printf("*********************************************************\n");
	printf("********************CRYPTOANALYSIS***********************\n");
	printf("*********************************************************\n");
	crypto_analysis(Y);
	printf("\n\n");


	// Decrypt the string
	printf("*********************************************************\n");
	printf("**********************DECRYPTION*************************\n");
	printf("*********************************************************\n\n");
	caesar_decrypt(Y, Z, k);
	printf("Decrypted string: %s\n", Z);


	// Free allocated memory
	free(X);
	free(Y);
	free(Z);

	return 0;
}
