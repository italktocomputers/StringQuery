#include "library.h"

int __PREFIX_substr(char* str, int start, int end, char* substr, int max) {
    int i = 0;
    char buffer[100];
    
    for (; i<=end-start; i++) {
        if (i >= max)
            return -1;
        
        if (str[start+i] == '\0')
            break;
        
        buffer[i] = str[start+i];
    }
    
    buffer[i] = '\0';
    
    strcpy(substr, buffer);
    
    return i;
}

void __PREFIX_urldecode(char encoded[], char* decoded) {
    int i = 0;
    int x = 0;
    
    while (encoded[i] != '\0') {
        // RFC1738: Space characters are replaced by '+'.
        if (encoded[i] == '+') {
            decoded[x] = ' ';
            i++;
        } else if (encoded[i] == '%') {
            // RFC1738: Non-alphanumeric characters are replaced by `%HH', a 
            // percent sign and two hexadecimal digits representing the ASCII 
            // code of the character.
            char hex[2] = {};
            hex[0] = encoded[i+1]; // The first hexadecimal digit
            hex[1] = encoded[i+2]; // The second hexadecimal digit
            
            char c = __PREFIX_hex_to_dec(hex, 2);
            
            decoded[x] = c;
            
            // Move 3 spaces since we just used %HH for our character.
            i += 3;
        } else {
            decoded[x] = encoded[i];
            i++;
        }
        
        x++;
    }
}

int __PREFIX_hex_to_dec(char hex[], int length) {
    int i = 0;
    int dec;
    int sum = 0;
    
    while (i <= length-1) {
        // Move from right to left
        switch (hex[length-1-i]) {
            case '0' :
                dec = 0;
                break;
            case '1' :
                dec = 1;
                break;
            case '2' :
                dec = 2;
                break;
            case '3' :
                dec = 3;
                break;
            case '4' :
                dec = 4;
                break;
            case '5' :
                dec = 5;
                break;
            case '6' :
                dec = 6;
                break;
            case '7' :
                dec = 7;
                break;
            case '8' :
                dec = 8;
                break;
            case '9' :
                dec = 9;
                break;
            case 'A' :
                dec = 10;
                break;
            case 'B' :
                dec = 11;
                break;
            case 'C' :
                dec = 12;
                break;
            case 'D' :
                dec = 13;
                break;
            case 'E' :
                dec = 14;
                break;
            case 'F' :
                dec = 15;
                break;
        }
        
        sum += (dec * (pow(16, i)));
        
        i++;
    }
    
    return sum;
}
