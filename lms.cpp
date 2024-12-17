#include<iostream>
#include<iomanip>
#include<string> 
#include<fstream>

using namespace std;
//validations
void lmsHeader(); //header
bool isValidCourseCode(string courseCode);
bool isValidCourseName(string courseName);
bool isValidCreditHours (int creditHour);
bool isValidSemester(int smester);
int getValidDigitInput();
//courses
void addCourse(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size);
void ViewCourses(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size);
void updateCourse(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size);
void deleteCourse(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size);
void courseOffer(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size);
bool saveCourses (string courseCode[],string courseName[], int creditHour[],int smester[],int size, bool isCourseExist[]);
bool loadCourses (string courseCode[],string courseName[], int creditHour[],int smester[],int size, bool isCourseExist[]);
string tokenizer(string str,int field); //comma tokenizer
int stringToInt(string str);
int main(){
    ofstream fout;
    ifstream fin;
    //course attributes
    int size = 50;
    string courseName[size] ;
    string courseCode[size];
    int smester[size];
    int creditHour[size];
    bool isCourseExist[size];
    bool isFound = false;
    int choice;
    //intilizations
    for(int i=0; i<size; i++){
        courseName[i] = "nv"; ;
        courseCode[i] = "nv";;
        smester[i] = -1;
        creditHour[i] = -1;
        isCourseExist[i] = false;
    }
    loadCourses(courseCode,courseName,creditHour,smester,size,isCourseExist);
    do{
        lmsHeader();
        choice = getValidDigitInput();
        switch (choice){
            case 1:{ //add course
                addCourse(courseName,courseCode,smester,creditHour,isCourseExist,size);
                break;
            }
            case 2:{ //update course
                updateCourse(courseName,courseCode,smester,creditHour,isCourseExist,size);
                break;
            }
            case 3:{ //delete course
                deleteCourse(courseName,courseCode,smester,creditHour,isCourseExist,size);
                break;
            }
            case 4:{ //view course
                ViewCourses(courseName,courseCode,smester,creditHour,isCourseExist,size);
                break;
            }
            case 5:{ //view course of smester
                courseOffer(courseName,courseCode,smester,creditHour,isCourseExist,size);
                break;
            }
            case 0:{ //exit
                break;
            }

            default:{
                cout<<"Invalid choice!\n";
                break;
            }
        }
    }while(choice!=0);
    if(saveCourses(courseCode,courseName,creditHour,smester,size,isCourseExist))
        cout<<"Data has been saved";
    return 0;
}
//<-------------------functionalities------------------>
bool saveCourses(string courseCode[],string courseName[], int creditHour[],int smester[],int size, bool isCourseExist[]){
    ofstream fout;
    fout.open("courses.txt");
    for(int i=0; i<size; i++){
        if(isCourseExist[i]){
            fout<<courseCode[i]<<","<<courseName[i]<<","<<creditHour[i]<<","<<smester[i]<<endl;
        }
    }
    fout.close();
    return 1;
}
bool loadCourses (string courseCode[],string courseName[], int creditHour[],int smester[],int size, bool isCourseExist[]){
    ifstream fin;
    fin.open("courses.txt");
    int indx = 0;
    string str = "";
    while(getline(fin, str)){
        if (str.empty()) 
            continue;
        courseCode[indx] = tokenizer(str, 1);
        courseName[indx] = tokenizer(str, 2);
        creditHour[indx] = stringToInt(tokenizer(str, 3));
        smester[indx] = stringToInt(tokenizer(str, 4));
        isCourseExist[indx] = true;
        indx++;
    }
    fin.close();
    if (indx == 0) {
        cout << "Warning: The file is empty or contains no valid data." << endl;
    }
    return 1;
}
string tokenizer(string str, int field){
    int commaCount = 0;
    string res = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ',')
            commaCount++; 
        else if (commaCount == field - 1)
            res += str[i]; 
        else if (commaCount >= field)
            break;
    }
    return res;
}
int stringToInt(string str){
    int result = 0;
    int i = 0; //loop itertor variable
    bool isNegative = false;
    if(str[0]=='-'){
        isNegative = true; //assures that num is really negative
        i = 1; //bcz at 0th indx -ve sign lies
    }
    for(i; i<str.length(); i++){
        int degit = str[i] - '0'; //means minus 48 ascii
        if(degit<0 || degit>9)
            return 0; //invalid string not contains numbers
        result = result * 10 + degit;
    }
    return result;
}
void addCourse(string courseName[],string courseCode[],int smester[],int creditHour[], bool isCourseExist[],int &size){
    bool added = false;
    for(int i=0; i<size; i++){
        if(!isCourseExist[i]){
            while(!added){
                cout<<"According to this Format : Course code,credit hours, smester, course name :\n";
                cout<<"Enter the details of the course: ";
                cin>>courseCode[i];
                cin>>creditHour[i];
                cin>>smester[i];
                getline(cin,courseName[i]);
                //check validation
                
                if(isValidCourseCode(courseCode[i]) && isValidCourseName(courseName[i]) && isValidCreditHours(creditHour[i]) && isValidSemester(smester[i])){
                    isCourseExist[i] = true;
                    added = true;
                    cout<<"Course has been added successfully"<<endl<<endl;
                    break;
                }
                else 
                    cout<<"Incorrect Format!"<<endl;
            }
        }
    }
    saveCourses(courseCode,courseName,creditHour,smester,size,isCourseExist);
}

void updateCourse(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size){
    string code;
    bool isFound = false;
    cout<<"Enter the course code : ";
    cin>>code;
    for(int i=0; i<size; i++){
        if(code==courseCode[i]){
            cout<<"Enter the details of the updated course: ";
            cin>>courseCode[i];
            cin>>creditHour[i];
            cin>>smester[i];
            getline(cin,courseName[i]);
            //check validation
            if(!isValidCourseCode(courseCode[i]) && !isValidCourseName(courseName[i]) && !isValidCreditHours(creditHour[i]) && !isValidSemester(smester[i])){
                cout<<"Incorrect format!"<<endl;
                i--;
                continue;
            }
            isFound = true;
            cout<<"Course has been edited successfully"<<endl;
            saveCourses(courseCode,courseName,creditHour,smester,size,isCourseExist);
        }
    }
    if(isFound)
        cout<<"Course not found!";

}

void deleteCourse(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size){
    string code;
    bool isFound = false;
    cout<<"Enter the course code : ";
    cin>>code;
    for(int i=0; i<size; i++){
        if(code==courseCode[i]){
            courseCode[i] = "nv";
            courseName[i] = "nv";
            smester[i] = -1;
            creditHour[i] = -1;
            isCourseExist[i] = false;
            isFound = true;
            saveCourses(courseCode,courseName,creditHour,smester,size,isCourseExist);
            cout<<"Course has been deleted successfully"<<endl;
        }
    }
    if(isFound)
        cout<<"Course not found!";
}
//view course
void ViewCourses(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size){
    cout<<left;
    system("cls");
    cout<<"--------------------------------------------------------------------------------"<<endl;
    cout<<setw(15)<<"Course Code"<<setw(30)<<"Name"<<setw(15)<<"Credit Hours"<<setw(15)<<"Semester"<<endl;
    cout<<"--------------------------------------------------------------------------------"<<endl;
    for(int i=0; i<size; i++){
        if(isCourseExist[i]){
            cout<<left<<setw(15)<<courseCode[i]<<setw(30)<<courseName[i]<<setw(15)<<creditHour[i]<<setw(15)<<smester[i]<<endl;
        }
    }   
    cin.get();
}
void courseOffer(string courseName[],string courseCode[],int smester[],int creditHour[],bool isCourseExist[],int &size){
   
   int smesterr;
    cout<<"Enter the semester: ";
    cin>>smesterr;
    cout<<"---------------------------------------------------------------------"<<endl;
    cout<<left<<setw(15)<<"Course Code"<<setw(30)<<"Name"<<setw(15)<<"Credit Hours"<<endl;
    cout<<"---------------------------------------------------------------------"<<endl;
    for(int i=0; i<size; i++){
        if(smester[i]==smesterr){
            cout<<left<<setw(15)<<courseCode[i]<<setw(30)<<courseName[i]<<setw(15)<<creditHour[i]<<endl;

        }
    }
}


//<---------------Validations------------------->
void lmsHeader(){
    cout<<" ***************************************************************"<<endl;
    cout<<"      Welcome to University Learning Management System            "<<endl;
    cout<<" ***************************************************************"<<endl;
    cout<<" Choose the following option"<<endl;
    cout<<" 1 Add Course"<<endl;
    cout<<" 2 Update Course"<<endl;
    cout<<" 3 Delete Course"<<endl;
    cout<<" 4 View All Courses"<<endl;
    cout<<" 5 View Courses of a Semester"<<endl;
    cout<<" 0 Exit Program "<<endl;

}
bool isValidCourseCode(string courseCode){
    if(courseCode.length()>5)
        return 0;
    bool alpha = true, digit = true;
    for(int i=0; i<2; i++){
        if(!((courseCode[i]>='a' && courseCode[i]<='z') || (courseCode[i]>='A' && courseCode[i]<='Z'))){
            alpha = false;
            break;
        }
    }
    for(int i=2; i<5; i++){
        if(!(courseCode[i]>='0' && courseCode[i]<='9')){
            digit = false;
            break;
        }
    }
    if(alpha && digit){
        return 1;
    }
    else{
        return 0;
    }
}
bool isValidCourseName(string courseName){
    if(courseName.length()>50)
        return 0;
    for(int i=0; i<courseName.length(); i++){
        if(!((courseName[i]>='a' && courseName[i]<='z') || (courseName[i]>='A' && courseName[i]<='Z') || (courseName[i]==' '))){
            return 0;
        }
    }
   return 1;
}
bool isValidCreditHours (int creditHour){
    if(creditHour>=1 && creditHour<=3)
        return 1;
    else
        return 0;
}
bool isValidSemester(int smester){
     if(smester>=1 && smester<=8)
        return 1;
    else
        return 0;
}
int getValidDigitInput(){
    int input = -1; //that will return the final correct input
    char choice[10];
    bool isDigit = false;
    do{
        
        // cout << "Select desired option: ";
        cin >> choice;
        cin.ignore(100,'\n');
        if((choice[0]>= '0' && choice[0]<='9') && choice[1]=='\0'){
            input = atoi(choice);
            return input;
        }
        else 
            return input;
            //that will return wrong input that detected by above used switch satement and shows error
            //after this again asked for input untill correct input
    }while(!isDigit);
}
