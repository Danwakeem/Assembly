/*
 * Name: Dan Jarvis
 * Class: CSCE 2610
 * Date: 8/1/14 
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//Print out all the Registers
void printRegs(int *regSet){
   int i;
   for(i = 0; i < 26; i++){
      printf("Reg: %2d Value: %d\n",i, regSet[i]);
   }
}

//This converts the hex to binary
void convertToBinary(char *buff){
   char ch;
   char binary[32+1];
   int i;
   int ohNo = 0;

   memset(binary,'\0',sizeof(binary));
 
   for(i = 0; i < strlen(buff); i++){
      ch = buff[i];
      switch(ch){
	 case '0': strcat(binary,"0000"); break;
         case '1': strcat(binary,"0001"); break;
	 case '2': strcat(binary,"0010"); break;
	 case '3': strcat(binary,"0011"); break;
	 case '4': strcat(binary,"0100"); break;
         case '5': strcat(binary,"0101"); break;
         case '6': strcat(binary,"0110"); break;
         case '7': strcat(binary,"0111"); break;
         case '8': strcat(binary,"1000"); break;
         case '9': strcat(binary,"1001"); break;
         case 'a':
	 case 'A': strcat(binary,"1010"); break;
	 case 'b':
  	 case 'B': strcat(binary,"1011"); break;
	 case 'c':
	 case 'C': strcat(binary,"1100"); break;
	 case 'd':
	 case 'D': strcat(binary,"1101"); break;
	 case 'e':
 	 case 'E': strcat(binary,"1110"); break;
	 case 'f':
  	 case 'F': strcat(binary,"1111"); break;
         default:
	    ohNo = 1;
            printf("Opps that character is not valid\n");
            break;
      }
      if(ohNo == 1)
 	 break;
   }
   if(ohNo != 1){
      memset(buff, '\0', sizeof(buff));
      strcpy(buff,binary);
   }

}

//This functions checks to see if it is the terminating mips line
//i.e ffffffff
int isF(char *buff){
   int i;
   for(i = 0; i < strlen(buff); i++){
      if(buff[i] != '1'){
         return 0;
      }
   }
   return 1;
}

//Taking the twos compliment of a binary value
void twosCompliment(char *index){
   //Flip and add 1
   int i;
   int foundOne = 0;
   char result[strlen(index)];
   memset(result,'\0',sizeof(result));
   for(i = (strlen(index) - 1); i >= 0; i--){
      if(foundOne == 1){
         if(index[i] == '1')
	    index[i] = '0';
	 else 
	    index[i] = '1';
      }
      else {
	 if(index[i] == '1')
	    foundOne = 1;
      }
   }
}

//Convert binary to decimal
int bToDecimal(char *index, char tCom){
   int i,final = 0;
   int power = 0;
   int add = 0;
   int j = 0;
   char ch;
   int isNegitive = 0;
   if(index[0] == '1' && tCom == 'y'){
      isNegitive = -1;
      twosCompliment(index);
   }
   for(i = (strlen(index) - 1); i >= 0; i--){
      add = 0;
      ch = index[i];
      add = atoi(&ch);
      if(add == 1){
 	 power = 0;
         power = (int)pow((double)2,(double)j);
 	 final += power;
      }
      j++;
   }
   if(isNegitive == -1)
      return (final * isNegitive);
   else
      return final;
}

//Get decimal value of the rs register
int getRsReg(char *buff, char type){
   int i, j = 0;
   char index[5+1];
   memset(index,'\0',sizeof(index));
   for(i = 6; i < 11; i++){
      index[j++] = buff[i];
   }
   i = bToDecimal(index, 'n');
   return i;
}

//I made a manual shift functions. You can take a look at them if you want but I 
//used to << and >> to do the shifts after I read a little bit about them.
/*
void shiftRight(char *index, int shamt){
   int i,j;
   for(i = 0; i < shamt; i++){
      for(j = 1; j < strlen(index); j++){
	 index[j-1] = index[j];
      }
      index[j] = '0';
   }
}

void shiftLeft(char *index, int shamt){
   int i,j;
   for(i = 0; i < shamt; i++){
      for(j = 0; j < (strlen(index) - 1); j++){
	 index[j+1] = index[j];
      }
      index[0] = '0';
   }
}

int shiftRtReg(char *buff, char direction, int shamt){
   int i, j = 0;
   char index[5+1];
   memset(index,'\0',sizeof(index));
   for(i = 11; i < 16; i++){
      index[j++] = buff[i];
   }
   if(direction == 'r')
      shiftRight(index,shamt);
   else
      shiftLeft(index,shamt);
   i = bToDecimal(index, 'n');
   return i;
}
*/

//Get the decimal value of the rt register
int getRtReg(char *buff){
   int i, j = 0;
   char index[5+1];
   memset(index,'\0',sizeof(index));
   for(i = 11; i < 16; i++){
      index[j++] = buff[i];
   }
   i = bToDecimal(index, 'n');
   return i;
}

//Get the decimal value of the rd register
int getRdReg(char *buff){
   int i, j = 0;
   char index[5+1];
   memset(index,'\0',sizeof(index));
   for(i = 16; i < 21; i++){
      index[j++] = buff[i];
   }
   i = bToDecimal(index, 'n');
   return i;
}

//Get the decimal value of the shift ammout.
int getShamt(char *buff){
   int i, j = 0;
   char index[5+1];
   memset(index,'\0',sizeof(index));
   for(i = 21; i < 26; i++){
      index[j++] = buff[i];
   }
   //ASK IF SHIFTS CAN BE -****
   i = bToDecimal(index, 'n');
   return i;
}

//Get the decimal value of the imediate value
int getImediate(char *buff){
   int i, j = 0; 
   char im[16+1];
   memset(im,'\0',sizeof(im));
   for(i = 16; i < 32; i++){
      im[j++] = buff[i];
   }
   i = bToDecimal(im, 'y');
   return i;
}

//Decimal version of the target address. I decide if it is a jump or a branch
//in the parse binary function.
int getTargetAddress(char *buff){
   int i,j = 0;
   char bTargetAddress[26+1];
   memset(bTargetAddress,'\0',sizeof(bTargetAddress));
   for(i = 6; i < strlen(buff); i++){
      bTargetAddress[j++] = buff[i];
   }
   i = bToDecimal(bTargetAddress,'n');
   return i;
}

//Get the function of R-Type to determine what to do with the register values
void getFunction(char *buff, char *func){
   int i, j= 0;
   for(i = 26; i < 32; i++){
      func[j++] = buff[i];
   }
}

//This is where the magic happens. I determine what to do with the binary instruction and then
//perform that instruction.
int parseBinary(char *buff,int *regSet,int *Pc, int *ind){
   char opcode[6+1];
   int i;
   int rs, rt, rd, sa, im, fun;
   int targ;
   memset(opcode,'\0',sizeof(opcode));
   for(i = 0; i < 6; i++){
      opcode[i] = buff[i]; 
   }
   //More parsing. All I know is that it is an R type
   if(strcmp(opcode,"000000") == 0){
      char func[6+1];
      memset(func,'\0',sizeof(func));
      getFunction(buff,func);
      rs = getRsReg(buff,'r');
      rt = getRtReg(buff);
      rd = getRdReg(buff);
      //Add
      if(strcmp(func,"100000") == 0){
 	 rs = regSet[rs];
	 rt = regSet[rt];
	 regSet[rd] = rs + rt;
         return 0;
      }
      //sub
      else if(strcmp(func,"100010") == 0){
 	 rs = regSet[rs];
	 rt = regSet[rt];
	 regSet[rd] = rs - rt;
         return 0;
      }
      //shift left
      else if(strcmp(func,"000000") == 0){
         sa = getShamt(buff);
	 rt = rs << sa;
 	 //rt = shiftRtReg(buff,'l',sa);
	 regSet[rd] = rt;
         return 0;
      }
      //shift right
      else if(strcmp(func,"000010") == 0){
         sa = getShamt(buff);
	 rt = rs >> sa;
 	 //rt = shiftRtReg(buff,'r',sa);
	 regSet[rd] = rt;
         return 0;
      }
      else {
	 printf("SorryBUB!\n");
      }	
   }
   //jump type
   else if(strcmp(opcode,"000010") == 0){
      int temp = 0;
      targ = getTargetAddress(buff);
      targ = targ * 4;
      temp = targ - *Pc;
      *Pc = targ;
      targ = temp / 4;
      *ind = *ind + (targ - 1);
      return targ;
   }
   //bne
   else if(strcmp(opcode,"000101") == 0){
      rs = getRsReg(buff, 'i');
      rt = getRtReg(buff);
      rs = regSet[rs];
      rt = regSet[rt];
      //If not equal branch
      if(rs != rt){
	 int temp = 0;
         im = getImediate(buff);
 	 temp = im * 4;
	 *Pc += temp;
	 *ind += im;
    	 temp = *Pc;
	 return temp;
         //Set the PC to the im and perform in structions from there.
      }
      return 0;
   }
   //beq
   else if(strcmp(opcode,"000100") == 0){
      rs = getRsReg(buff, 'i');
      rt = getRtReg(buff);
      rs = regSet[rs];
      rt = regSet[rt];
      //If equal then branch
      if(rs == rt){
	 int temp = 0;
         im = getImediate(buff);
 	 temp = im * 4;
	 *Pc += temp;
	 ind += im;
    	 temp = *Pc;
	 return temp;
         //Set the PC to the im and perform in structions from there.
      }
      return 0;
   }
   //addi
   else if(strcmp(opcode,"001000") == 0){
      rs = getRsReg(buff, 'i');
      rt = getRtReg(buff);
      im = getImediate(buff);
      rs = regSet[rs];
      regSet[rt] = rs + im;
      return 0;
   }
   //slti
   //If s < imediate set rt = 1
   else if(strcmp(opcode,"001010") == 0){
      rs = getRsReg(buff, 'i');
      im = getImediate(buff);
      rt = getRtReg(buff);
      rs = regSet[rs];
      //If rs < im then set rt register to 1
      if(rs < im)
	 regSet[rt] = 1;
      else 
	 regSet[rt] = 0;
      return 0;
   }
   //This is the special print opcode that the professor wanted.
   else if(strcmp(opcode,"111111") == 0){
      printRegs(regSet);
   }
   //If opcode is invalid tell the user.
   else {
      printf("invalid opcode\n");
      return -1;
   }
}

//Initilization of the registers. I just set them all to 0 initially.
void iniRegs(int *regSet){
   int i;
   for(i = 0; i < 26; i++){
      regSet[i] = 0;
   }
}

//Get the number of lines in the file.
//I didn't feel like making a linked list of the instructions so I just figured out how many
//lines were in the file then malloced an array to hold all the instructions. I know it is crappy
//and inefficent but I had never rewinded on a file before so I wanted to try it.
int getNumLines(FILE *read){
   int i = 0;
   char blah[100+1];
   while(fgets(blah,sizeof(blah),read))
      i++;
   return (i - 1);
}

//Free all the space I malloced.
void releaseInstructions(char **in, int numLines){
   int i,j;
   for(i = 0; i < numLines; i++){
      free(in[i]);
   }
   free(in);
}

int main(){
   char buff[100+1];
   char **instructions;
   char address[16+1];
   char fileName[100+1];
   char begin;
   int regSet[36];
   int i,j,Pc;
   FILE *read;
   int numLines = 0;

   //Initialize the registers
   iniRegs(regSet);

   //Ask user for the file name.
   printf("Please enter the name of your input file: \n");
   scanf("%s",fileName);

   //Open the file
   read = fopen(fileName, "r");
   //Get number of lines in the file
   numLines = getNumLines(read);
   //Rewind to the beginning of the file. Isn't that fancy.
   rewind(read);

   //Malloc the number of lines.
   instructions = (char**) malloc(numLines * sizeof(char*));
   //Set a buffer size big enough to hold the binary. I went a little over board on this 
   for(i = 0; i < numLines; i++){
      instructions[i] = (char*) malloc(101 * sizeof(char));
      memset(instructions[i],'\0',sizeof(instructions[i]));
   }
   //Make sure the file starts with an address.
   fscanf(read,"%c", &begin);

   if(begin != '#'){
      printf("You must start the address of the first line.\n");
   }
   //Convert address to binary.
   fgets(address,sizeof(address), read);
   address[strlen(address) - 1] = '\0';
   convertToBinary(address);

   //Read in instructions.
   i = 0;
   while(fgets(buff,sizeof(buff),read) != NULL){
      strcpy(instructions[i],buff);
      instructions[i][strlen(instructions[i]) - 1] = '\0';
      convertToBinary(instructions[i]);
      i++;
   }
   //Close file
   fclose(read);

   //Pc as a decimal value
   Pc = bToDecimal(address,'n');      
   j = 0;
   //Read each instruction then parse the instruction.
   for(i = 0; i < numLines; i++){
      printf("Pc: %d   instructions: %s\n",Pc, instructions[i]);
      if(isF(instructions[i]) == 1){
         releaseInstructions(instructions,numLines);
	 return 0;
      }
      j = parseBinary(instructions[i],regSet,&Pc,&i);
      if(j == -1){
	 j = 0;
      }
      else if(j == 0){
         Pc += 4;
      }
      j = 0;
   }
   printf("****** Dropped off bottom *****\n");
   return 0;

}
