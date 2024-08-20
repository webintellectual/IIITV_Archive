/* © Author: Akshay | 202051018 */

#include<stdio.h>
#include<stdint.h>
#define IRRED_POLY 0x11B

unsigned char sub_table[16][16] = {
   {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
   {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
   {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
   {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
   {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
   {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
   {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
   {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
   {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
   {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
   {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
   {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
   {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
   {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
   {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
   {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

uint32_t RCon[]={
   0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 
   0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000
};

// Subbyte function // tested Ok
unsigned char Subbyte(unsigned char a){
   int t1, t2;
   t1 = a & 15;
   t2 = a >> 4;

   return sub_table[t2][t1];
}

// Inverse SubByte function
unsigned char SubbyteInverse(unsigned char a){
   int i , j;
   unsigned char x;
   for (i = 0; i < 16; i++){
      for(j = 0; j < 16; j++){
         if(sub_table[i][j] == a){
            x = i;
            x = (x << 4) | j;
            break;
         }
      }    
   }
   return x;
}

void shiftrows(uint8_t s[4][4]){
   uint8_t t1,t2,t3;

   t1 = s[1][0];
   s[1][0] = s[1][1];
   s[1][1] = s[1][2];
   s[1][2] = s[1][3];
   s[1][3] = t1;

   t1=s[2][0], t2=s[2][1];
   s[2][0] = s[2][2];
   s[2][1] = s[2][3];
   s[2][2] = t1;
   s[2][3] = t2;

   t1=s[3][0], t2=s[3][1], t3=s[3][2];
   s[3][0] = s[3][3];
   s[3][1] = t1;
   s[3][2] = t2;
   s[3][3] = t3;
}

void shiftrowsInverse(uint8_t s[4][4]){
   uint8_t t1,t2,t3;

   t1 = s[1][3];
   s[1][3] = s[1][2];
   s[1][2] = s[1][1];
   s[1][1] = s[1][0];
   s[1][0] = t1;

   t1=s[2][3], t2=s[2][2];
   s[2][3] = s[2][1];
   s[2][2] = s[2][0];
   s[2][1] = t1;
   s[2][0] = t2;

   t1=s[3][3], t2=s[3][2], t3=s[3][1];
   s[3][3] = s[3][0];
   s[3][2] = t1;
   s[3][1] = t2;
   s[3][0] = t3;
}

uint8_t gf_mult(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    uint8_t carry;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            result ^= a;
        }
        carry = a & 0x80;
        a <<= 1;
        if (carry) {
            a ^= IRRED_POLY;
        }
        b >>= 1;
    }
    return result;
} 

// Subbyte' function // tested Ok
unsigned char SubbyteDash(unsigned char a){
   return Subbyte(gf_mult(2,a)^1);
}

unsigned char SubbyteDashInverse(unsigned char a){
   return gf_mult(141,SubbyteInverse(a)^1);
}

void mixColumn(u_int8_t sMatrix[4][4]){
   uint8_t outpt[4][4];
   uint8_t M[4][4] = {
      {1, 4, 4, 5},
      {5, 1, 4, 4},
      {4, 5, 1, 4},
      {4, 4, 5, 1}
   };
   for(int c=0; c<4; c++){
      // generate cth column of output matrix:-
      for(int i=0; i<4; i++){ // iterating to rows of m
         outpt[i][c] = gf_mult(M[i][0],sMatrix[0][c])^gf_mult(M[i][1],sMatrix[1][c])^gf_mult(M[i][2],sMatrix[2][c])^gf_mult(M[i][3],sMatrix[3][c]);
      }
   }
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         sMatrix[i][j] = outpt[i][j];
      }
   }
}

void mixColumnInverse(uint8_t sMatrix[4][4]){
   uint8_t outpt[4][4];
   uint8_t Mi[4][4] = {
      {165, 7, 26, 115},
      {115, 165, 7, 26},
      {26, 115, 165, 7},
      {7, 26, 115, 165}
   };
   for(int c=0; c<4; c++){
      // generate cth column of output matrix:-
      for(int i=0; i<4; i++){ // iterating to rows of m
         outpt[i][c] = gf_mult(Mi[i][0],sMatrix[0][c])^gf_mult(Mi[i][1],sMatrix[1][c])^gf_mult(Mi[i][2],sMatrix[2][c])^gf_mult(Mi[i][3],sMatrix[3][c]);
      }
   }
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         sMatrix[i][j] = outpt[i][j];
      }
   }
}

uint32_t rotword(uint32_t word){
   uint32_t temp = word;
   temp = temp<<8;
   return temp | (word>>24);
}


uint32_t subword(uint32_t input){
   uint32_t ans = 0;
   uint32_t mask = 0xFF;
   ans = ans | (input&mask);
   for(int i=0; i<4; i++){
      ans = ans | ( Subbyte((input&mask)>>(8*i)) << (8*i));
      mask = mask<<8;
   }
   return ans;
}

void wordGenerator(uint8_t key[4][4], uint32_t word[]){
   for(int i=0; i<=3; i++){
      word[i] = ( ((uint32_t)key[0][i]) <<24 ) | ( ((uint32_t)key[1][i])<<16 ) | ( ((uint32_t)key[2][i])<<8 ) | ((uint32_t)key[3][i]);
   }
   for(int i=4; i<=43; i++){
      uint32_t temp = word[i-1];
      if((i%4)==0)
      temp = subword(rotword(temp))^RCon[i/4];
      word[i] = word[i-4]^temp;
   }
}

void keyScheduler(uint8_t key[4][4],  uint8_t roundKeys[11][4][4]){
   uint32_t word[44];
   wordGenerator(key,word);
   for(int i=0; i<11; i++){ // one key in each iteration
      // word[4*i] word[4*i+1] word[4*i+2] word[4*i+3]
      for(int w=0; w<4; w++){ // processing one word out of 4 words at a time
         uint32_t wrd = word[4*i+w];
         uint32_t mask = 0x000000FF;
         // Now we will extract four pieces of size 8-bit each from this word
         roundKeys[i][3][w] = (wrd&mask); 
         mask = mask<<8;
         roundKeys[i][2][w] = (wrd&mask)>>8;
         mask = mask<<8;
         roundKeys[i][1][w] = (wrd&mask)>>16;
         mask = mask<<8;
         roundKeys[i][0][w] = (wrd&mask)>>24;
      }
   }
}


void display(uint8_t text[4][4]){
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         printf("%X ",text[i][j]);
      }
      printf("\n");
   }
   printf("\n");
}

void encryption(uint8_t input[4][4], uint8_t key[4][4], uint8_t ciphertext[4][4]){
   uint8_t roundKey[11][4][4];
   keyScheduler(key, roundKey);
   
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         ciphertext[i][j] = input[i][j]^roundKey[0][i][j];
      }
   }

   // round 1 to 9
   for(int r=1; r<=9; r++){
      // Applying SubbyteDash:
      for(int i=0; i<4; i++){
         for(int j=0; j<4; j++){
            ciphertext[i][j] = SubbyteDash(ciphertext[i][j]);
         }
      }
      // Applying shift rows:
      shiftrows(ciphertext);
      //Applying Mix column:
      mixColumn(ciphertext);

      for(int i=0; i<4; i++){
         for(int j=0; j<4; j++){
            ciphertext[i][j] = ciphertext[i][j]^roundKey[r][i][j];
         }
      }
      // if(r==8) display(ciphertext);
   }

   // round 10:
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         ciphertext[i][j] = SubbyteDash(ciphertext[i][j]);
      }
   }
   shiftrows(ciphertext);
   for(int i=0; i<4; i++){
         for(int j=0; j<4; j++){
            ciphertext[i][j] = ciphertext[i][j]^roundKey[10][i][j];
         }
      }
}

void decryption(uint8_t input[4][4], uint8_t key[4][4], uint8_t plaintext[4][4]){
   uint8_t roundKey[11][4][4];
   keyScheduler(key, roundKey);
   
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         plaintext[i][j] = input[i][j]^roundKey[10][i][j];
      }
   }

    // round 10:
    shiftrowsInverse(plaintext);
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         plaintext[i][j] = SubbyteDashInverse(plaintext[i][j]);
      }
   }

   // round 9 to 1
   for(int r=9; r>=1; r--){
      // if(r==8) display(plaintext);
      for(int i=0; i<4; i++){
         for(int j=0; j<4; j++){
            plaintext[i][j] = plaintext[i][j]^roundKey[r][i][j];
         }
      }
      mixColumnInverse(plaintext);
      shiftrowsInverse(plaintext);
      for(int i=0; i<4; i++){
         for(int j=0; j<4; j++){
            plaintext[i][j] = SubbyteDashInverse(plaintext[i][j]);
         }
      }
   }

   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         plaintext[i][j] = plaintext[i][j]^roundKey[0][i][j];
      }
   }

}
int main(){
   // uint8_t plaintext[4][4] = {
   //    {0x81, 0xd3, 0xf2, 0xe0}, 
   //    {0x82, 0x81, 0xff, 0xed},
   //    {0x9c, 0x67, 0x67, 0xbb}, 
   //    {0xa6, 0xc9, 0x8a, 0x12}
   //  };
   uint8_t plaintext[4][4];
   printf("Enter plaintext: \n");
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         scanf("%hhx",&plaintext[i][j]);
      }
   }
   //42 26 45 29 48 40 4D 63 51 66 54 6A 57 6E 5A 72
   // scanf("%x",&hex);
   // uint8_t MainKey[4][4] = {
   //    {0xab, 0xba, 0xb0, 0x24}, 
   //    {0x12, 0xd7, 0x97, 0x39}, 
   //    {0x8c, 0x53, 0xb6, 0xac}, 
   //    {0xe9, 0x26, 0xb1, 0x12}
   // };


   uint8_t MainKey[4][4];
   printf("Enter key: \n");
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         scanf("%hhx", &MainKey[i][j]);
      }
   }

   printf("Plaintext: \n");
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         printf("%X ",plaintext[i][j]);
      }
      printf("\n");
   }

   printf("\n");

   uint8_t ciphertext[4][4];
   encryption(plaintext,MainKey,ciphertext);
   printf("Ciphertext: \n");
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         printf("%X ",ciphertext[i][j]);
      }
      printf("\n");
   }

   printf("\n");

   uint8_t decOut[4][4];
   decryption(ciphertext,MainKey,decOut);
   printf("Decryption Output: \n");
   for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
         printf("%X ",decOut[i][j]);
      }
      printf("\n");
   }
   return 0;
}