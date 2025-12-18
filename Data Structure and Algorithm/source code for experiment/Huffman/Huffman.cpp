#include <stdio.h>
#include "C:\Users\TX\Desktop\Data Structure\include\Huffman.h"

int main() {
    int choice;
    HuffmanTree HT = nullptr;
    HuffmanCode *w = nullptr;
    int n = 0;
    while (1) {
        printf("==================== Huffman Tree =================\n");
        printf("1.Intialization: Create a Huffman tree from weights array w[1..n]\n");
        printf("2.Encoding: Generate Huffman codes for each weight\n");
        printf("3.Decoding: Decode a given Huffman encoded string\n");
        printf("4.Printing\n");
        printf("5.TreePrinting: Print the structure of the Huffman tree\n");
        printf("6.Caculate compression ratio\n");
        printf("7.Exit\n");
        printf("===================================================\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);
        getchar(); // Consume newline after number input
        switch (choice) {
            case 1: {
                FILE *fp = fopen("stdio.txt", "r");
                if (!fp) {
                    printf("Cannot open file stdio.txt !\n");
                    break;
                }
                int x = count(*fp);
                n = x;
                HT = new HTNode[2 * n];
                countfrequency(w, fp, x);
                fclose(fp);
                CreateHuffmanTree(HT, w, x);
                printf("Huffman Tree created successfully.\n");
                break;
            }
            case 2: {
                if (HT == nullptr) {
                    printf("Please initialize the Huffman tree first.\n");
                    break;
                }
                char **HC = new char*[n + 1]; // 下标1~n
                Encoding(HT, HC, n);
                FILE *fp = fopen("ToBeTran.txt", "r");
                FILE *fp1 = fopen("CodeFile.txt", "w");
                if (!fp) {
                    printf("Cannot open file ToBeTran.txt !\n");
                    delete[] HC;
                    break;
                }
                if (!fp1) {
                    printf("Cannot open file CodeFile.txt !\n");
                    fclose(fp);
                    delete[] HC;
                    break;
                }
                char ch;
                while ((ch = fgetc(fp)) != EOF) {
                    for (int i = 0; i < n; i++) {
                        if (ch == w[i].ch) {
                            fprintf(fp1, "%s", HC[i + 1]); // HC[1]~HC[n]对应w[0]~w[n-1]
                            break;
                        }
                    }
                }
                fclose(fp);
                fclose(fp1);
                // 释放HC
                for (int i = 1; i <= n; i++) {
                    delete[] HC[i];
                }
                delete[] HC;
                printf("Encoding finished. Result written to CodeFile.txt\n");
                break;
            }
            case 3: {
                if (HT == nullptr) {
                    printf("Please initialize the Huffman tree first.\n");
                    break;
                }
                FILE *fp = fopen("CodeFile.txt", "r");
                if (!fp) {
                    printf("Cannot open file CodeFile.txt !\n");
                    break;
                }
                fseek(fp, 0, SEEK_END);
                long fileSize = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                char *encodedStr = new char[fileSize + 1];
                fread(encodedStr, 1, fileSize, fp);
                encodedStr[fileSize] = '\0';
                fclose(fp);
                Decoding(HT, encodedStr, n);
                printf("Decoding finished. Result written to TextFile.txt\n");
                delete[] encodedStr;
                break;
            }
            case 4: {
                FILE *fp = fopen("CodeFile.txt", "r");
                if (!fp) {
                    printf("Cannot open file CodeFile.txt !\n");
                    break;
                }
                fseek(fp, 0, SEEK_END);
                long fileSize = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                char *encodedStr = new char[fileSize + 1];
                fread(encodedStr, 1, fileSize, fp);
                encodedStr[fileSize] = '\0';
                printf("Encoded string from CodeFile.txt:\n");
                fclose(fp);
                for (int i = 0; i < fileSize; i++) {
                    if((i+1) % 50 != 0){
                        printf("%c", encodedStr[i]);
                    } else if((i+1) % 50 == 0){
                        printf("\n"); 
                    }
                }
                char *print = new char[fileSize + 1000000];
                int k = 0;int j = 0;
                while (encodedStr[k] != '\0') {
                    print[j] = encodedStr[k];
                    if ((j+1) % 50 == 0) {
                        print[j] = '\n';
                        k = k-1;
                    }
                    k++;j++;
                }
                print[j+1] = '\0';
                FILE *fp1 = fopen("CodePrint.txt", "w");
                if (!fp1) {
                    printf("Cannot open file CodePrint.txt !\n");
                    delete[] encodedStr;delete[] print;
                    break;
                }
                fprintf(fp1, "%s", print);
                fclose(fp1);
                printf("\nEncoded string also written to CodePrint.txt\n");
                delete[] encodedStr;delete[] print;
                break;
            }
            case 5: {
                if (HT == nullptr) {
                    printf("Please initialize the Huffman tree first.\n");
                    break;
                }
                TreePrint(HT, n);
                break;
            }
            case 6: {
                FILE *fp = fopen("stdio.txt", "r");
                if (!fp) {
                    printf("Cannot open file stdio.txt !\n");
                    break;
                }
                fseek(fp, 0, SEEK_END);
                long originalSize = ftell(fp);
                fclose(fp);

                fp = fopen("CodeFile.txt", "r");
                if (!fp) {
                    printf("Cannot open file CodeFile.txt !\n");
                    break;
                }
                fseek(fp, 0, SEEK_END);
                long encodedSize = ftell(fp);
                encodedSize = encodedSize / 8; 
                fclose(fp);

                if (originalSize == 0) {
                    printf("Original file is empty, cannot calculate compression ratio.\n");
                    break;
                }

                double ratio = static_cast<double>(originalSize) / static_cast<double>(encodedSize);
                printf("Original size: %ld bytes\n", originalSize);
                printf("Encoded size: %ld bytes\n", encodedSize);
                printf("Compression ratio: %.2f%%\n", ratio * 100.0);
                break;
            }
            case 7:
                if (w) delete[] w;
                printf("Exit.\n");
                return 0;
            default:
                printf("Exit.\n");
                return 0;
        }
    }
    if (w) delete[] w;
    return 0;

}
