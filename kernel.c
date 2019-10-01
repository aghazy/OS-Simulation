void printString(char*);
void readString(char*);
int div(int , int);
int mod(int, int);
void readSector(char* , int);
void handleInterrupt21(int , int , int , int );
int readFile(char*,char*);
void executeProgram(char* , int);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char* , char*, int);
int main()
{
	/* code */
	// char line[82];
 //     printString("Enter a charactec: \0"); 
	// readString(line);
	// printString(line);
	
	// char buffer[512];
	// readSector(buffer, 30);
	// printString(buffer);
	// makeInterrupt21();
	// interrupt(0x21,0,0,0,0);
	// char line[512];
	
	// makeInterrupt21();
	// interrupt(0x21,1,line,0,0);
	// interrupt(0x21,0,line,0,0);
	// interrupt(0x21,2,line,30,0);
	// interrupt(0x21,0,line,0,0);
	// while(1);
	// char buffer[13312]; /*this is the maximum size of a file*/
	// makeInterrupt21();
	// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
	// while(1);
	// makeInterrupt21();
	// interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
	// while(1);
	// char buffer[13312];
	// makeInterrupt21();
	// interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
	// interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
	// // interrupt(0x21, 0, buffer, 0, 0);
	// int i=0;
	// char buffer1[13312];
	// char buffer2[13312];
	// buffer2[0]='h'; 
	// buffer2[1]='e';
	// buffer2[2]='l';
	// buffer2[3]='l';
	// buffer2[4]='o';
	// for(i=5; i<13312; i++) buffer2[i]=0x0;
	makeInterrupt21();
	// interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
	interrupt(0x21,4, "shell\0", 0x2000, 0); //read file testW
	// interrupt(0x21,4, buffer1, 0, 0); // print out contents of testW
	
	while(1);
	return 0;
}
void printString(char* chars){

	while((*chars)!='\0'){
		// interrupt(0x10,0xE*256+'G',0,0,0);
		interrupt(0x10,0xE*256+*chars,0,0,0);
		chars++;
	}
}
void readString(char* line){
	 
	 char c[3];
	 c[0]='\0';
		
	while(c[0]!=0x64){
	 c[1]=interrupt(0x16,0,0,0,0);
	 if(c[1]==0xd){
	 	*line=0xa;
	 	line++;
	 	*line=0x0;
	 	line++;
	 	c[0]=0x64;

	 }else{
	 	
	 	if(c[1]==0x8){

	 		if (c[0]>0){
	 		interrupt(0x10,0xE*256+c[1],0,0,0);
	 		interrupt(0x10,0xE*256+' ',0,0,0);
	 		interrupt(0x10,0xE*256+0x8,0,0,0);
	 		line--;
	 		c[0]--;
	 	}
	 
	 }else{
	 	interrupt(0x10,0xE*256+c[1],0,0,0);
	 	c[0]=c[0]+1;
	 	*line=c[1];
	 	line++;
	 }
	 	
	}
	}
}
int div(int x, int y){
	int res =0;
	while(x>=y){
		res++;
		x=x-y;
	}
	return res;

}
int mod(int x,int y){
	int res =x-(div(x,y))*y;
	return res;
}
void readSector(char* buffer, int sector){
	int cl=mod(sector,18)+1;
	int dh =mod(div(sector ,18),2);
	int ch = div(sector,36);
	interrupt(0x13 , 2*256+1 , buffer , ch*256+cl , dh*256+0);
}
int readFile(char* name, char* buffer){
	char directory[512];
	int i;
	readSector(directory,2);
	
	i=0;
	while(i<512){
		int x=1;
		if(directory[i]!=0x0){
			int k =0;
			while(k<6 && name[k]!='\0'&& directory[i+k]!='\0'){
				if(name[k]!=directory[i+k]){
					x=0;
					break;
				}
				k++;
			}
			if(x!=0){
					int k =i+6;
					while(directory[k]!=0x0){
						readSector(buffer,directory[k]);
						buffer+=512;
						k++;
					}
				}

		}
		i=i+32;
	}return 0;
	

	
}
void executeProgram(char* name, int segment){
	char buffer[13312];
	int c=0x0000;
	int i=0;
	readFile(name,buffer);
	while(i<13312){
		putInMemory(segment,c,buffer[i]);
		// printString(c);
		//buffer++;
		i++;
		c++;
	}
	launchProgram(segment);

}
void terminate(){

	char buffer[7];
	buffer[0]='s';
	buffer[1]='h';
	buffer[2]='e';
	buffer[3]='l';
	buffer[4]='l';
	buffer[5]='\0';
	makeInterrupt21();
	interrupt(0x21,4,buffer, 0x2000, 0);
}
void writeSector(char* buffer, int sector){
	int cl=mod(sector,18)+1;
	int dh =mod(div(sector ,18),2);
	int ch = div(sector,36);
	interrupt(0x13 , 3*256+1 , buffer , ch*256+cl , dh*256+0);
}
void deleteFile(char* name){
	char directory[512];
	char map[512];
	int i =0;
	readSector(map,1);
	readSector(directory,2);
	
	while(i<512){
		int x=1;
				if(directory[i]!=0x0){
			int k =0;
			while(k<6){
				if(name[k]!=directory[i+k]){
					x=0;
					break;
				}
				k++;
			}
			if(x!=0){
				directory[i]=0x00;
					k =i+6;
					while(directory[k]!=0x0){
						int index=directory[k];
						map[index]=0x0;
						k++;
					}
					writeSector(map,1);
					writeSector(directory,2);
					return;
					}
		
		}
		i=i+32;
	}
	
}
void writeFile(char* name, char* buffer, int secNum){
	char directory[512];
	char map[512];
	int i =0;
	readSector(map,1);
	readSector(directory,2);
	while(i<512){
		if(directory[i]==0x0){
			int k =0;
			int x=4;
			int counter=0;
			int y=0;
			while(k<6){
				if(*name!='\0'){
					directory[i+k]=*name;
					name++;
				}else{
					directory[i+k]=0x00;
				}
				k++;
			}
			while(x<512&&counter<secNum){
				if(map[x]==0x0){
					
					counter++;
				}
				x++;
			}
			if(counter<secNum){
				printString("ERROR: There is no space!!");
				return;
			}
			x=4;
			counter=0;
			while(x<512&&counter<secNum){
				char tmp[512];
				if(map[x]==0x0){
					map[x]=0xff;
					y=0;
					
					while(y<512){
						tmp[y]=*buffer;
						buffer++;
						y++;
					}
					writeSector(tmp,x);
					directory[i+k+counter]=x;
					counter++;
				}
				x++;
			}
			while((k+counter)<32){
				directory[i+k+counter]=0x00;
				counter++;
			}
			writeSector(map,1);
			writeSector(directory,2);
			return;
		}
		i=i+32;
	}
	printString("ERROR: There is no space!!");


	
}
void handleInterrupt21(int ax, int bx, int cx, int dx){
	
	if(ax==0){
		printString(bx);
	}else{
		if(ax==1){
			readString(bx);
		}else{
			if(ax==2){
				readSector(bx,cx);
			}else{
				if(ax==3){
					readFile(bx,cx);
				}else{
					if(ax==4){
						executeProgram(bx,cx);
					}else{
						if(ax==5){
						terminate();
					}else{
						if(ax==6){
							writeSector(bx,cx);

						}else{
							if(ax==7){
								deleteFile(bx);
							}else{
								if(ax==8){
									writeFile(bx, cx, dx);
								}else{
									printString("ERROR YOU ARE WRONG!!!!!");			
								}
							
						}}
							
					}}
	
				
			}
			}
		}
	}
}
