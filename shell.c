int equals(char*,char*);
int equals2(char*,char*);
void seperate(char*,char*,char*);
int size(char*);
void print2(int);
int div(int , int);
int mod(int, int);
void write(char*);
void truncate(char* int);
int main(){
	
		char buffer[82];
		
		char buffer2[13312];
		
		char* x = buffer;
		truncate(buffer2, 13312);
		truncate(buffer, 82);
		
		// makeInterrupt21();
		interrupt(0x21,0,"SHELL> ", 0, 0);
		interrupt(0x21,1,buffer,0,0);
		interrupt(0x21,0,"\n\r",0,0);
		// interrupt(0x21,0,buffer,0,0);
		// interrupt(0x21,0,"\n\r",0,0);
		// interrupt(0x21,0,buffer2,0,0);
		// interrupt(0x21,0,"\n\r",0,0);
		
		if(equals("view\0",buffer)){
			x+=5;
			interrupt(0x21,3,x,buffer2,0);
			if(buffer2[0]==0){//0x0
				
					interrupt(0x21,0,"ERROR: File not Found!!\n\r", 0, 0);
				}else{
			interrupt(0x21,0,buffer2,0,0);}
			interrupt(0x21,0,"\r",0,0);
		}else{if(equals("execute\0",buffer)){
			x+=8;
			interrupt(0x21, 4, x, 0x2000, 0);

		}else{
			if(equals("delete\0",buffer)){
				x+=7;
				interrupt(0x21,3,x,buffer2,0);
				if(buffer2[0]==0x0){
					interrupt(0x21,0,"ERROR: File not Found!!\n\r", 0, 0);
				}else{
					interrupt(0x21, 7, x, 0, 0);	
				}
				
				
			}else{
				if(equals("copy\0",buffer)){
					char buffer1[82];
					char buffer3[82];
					x+=5;
					truncate(buffer1, 82);
					truncate(buffer3, 82);
					seperate(x,buffer1,buffer3);
					interrupt(0x21,3,buffer1,buffer2,0);
					// interrupt(0x21,0,buffer3,0,0);
				if(buffer2[0]==0){//0x0
					interrupt(0x21,0,"ERROR: File not Found!!\n\r", 0, 0);
				}else{
					if(buffer3[0]=='\0'){

						interrupt(0x21,0,"BAD COMMAND\n\r",0,0);		
					}else{
						int s = size(buffer2);
						
						interrupt(0x21, 8, buffer3, buffer2, s);
					}
						
				}
				}else{
					if(equals2("dir\0",buffer)){
						char directory[512];
						int i=0;
						interrupt(0x21,2,directory,2,0);
						while(i<512){
							if(directory[i]!=0x0){
								char name[7];
								int c=0;
								int k=0;
								while(k<6){
									name[k]=directory[i+k];
									k++;

								}
								name[6]='\0';
								interrupt(0x21,0,name, 0, 0);
								interrupt(0x21,0,": ", 0, 0);
								while(directory[i+k]!=0x0&&k<32){
									k++;
									c++;
								}


								print2(c);
								interrupt(0x21,0,"\n\r",0,0);
							}
							i+=32;
						}
					}else{
						if(equals("create\0",buffer)){
							int sz;
							x+=7;
							write(buffer2);
							sz=size(buffer2);
							interrupt(0x21,8,x, buffer2, sz);
							interrupt(0x21,0,"\n\r",0,0);


						}else{
							interrupt(0x21,0,"BAD COMMAND\n\r",0,0);	
						}
									
					}
					
				}
				
			}
			
		}
		
		}
	interrupt(0x21,4, "shell\0", 0x2000, 0);
	
}
	int equals(char* command,char* input){
		while(1){
			if(*input==' '&&*command=='\0'){
				return 1;
			}
			if(*input!=*command){
				return 0;
			}
			input++;
			command++;
		}
		return 0;
	}
	int equals2(char* command,char* input){
		while(1){
				if(*command=='\0'&& *input=='\n'){
				return 1;
			}
			if(*input!=*command){
				return 0;
			}
		
			input++;
			command++;
		}
		return 1;
	}
	void seperate(char* origin, char* buffer1,char* buffer2){
		
		while(*origin!=' '){
			*buffer1=*origin;
			buffer1++;
			origin++;
			if(*origin=='\n'){
				return;
			}
		}
		if(*origin=='\0'){
			return;
		}
		origin++;
		while(*origin!='\0'){
			*buffer2=*origin;
			buffer2++;
			origin++;	
		}
		// buffer2--;
		// buffer2--;
		*buffer1='\n';
		*buffer2='\0';
	}
 int size(char* buffer){
 	int i=0;
 	while(*buffer!='\0'){
 		i++;
 		buffer+=512;
 	}
 	return i;
 }
 void print2(int i){
 	int index=0;
 	int x=0;
 	char result[6];
 	char final[6];
 	while(i!=0){
 		char c = '0'+mod(i,10);
 		result[index]=c;
 		index++;
 		i=div(i,10);
 	}
 		// if(index==0){
 		// 	result[index]=0;
 		// 	index++;
 		// }
 	result[index]='\0';
 	index--;
 	while(result[x]!='\0'){
 		final[index]=result[x];
 		x++;
 		index--;
 	}
 	final[x]='\0';
 	interrupt(0x21,0,final,0,0);
 		
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
void write(char* buffer){
	char tmp[82];
	char* x;
	while(1){
		x=tmp;
		interrupt(0x21,1,tmp,0,0);
		if(tmp[0]=='\n'){
			break;
		}
		while(*x!='\0'){
			*buffer = *x;
			buffer++;
			x++;
		}
		*buffer='\r';
		buffer++;
		interrupt(0x21,0,"\n\r",0,0);	
	}


}
void truncate(char* buffer, int x){
	int i =0;
	while(i<x){
		buffer[i]=0x00;
		i++;
	}
}