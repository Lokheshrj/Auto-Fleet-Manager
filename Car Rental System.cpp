#include<iostream>
#include<stdio.h>
#include<cstring>
#include<fstream>
#define MAX 30
#define DURATION 3
using namespace std;
typedef struct dum
{
    char name[MAX],mobile[MAX],address[MAX],license[MAX],car[MAX],car_no[MAX];
    unsigned int age;
    struct dum *link;
}customer;
typedef struct samp
{
    char name[MAX],mobile[MAX],address[MAX],license[MAX];
    unsigned int age;
    struct samp *link;
}waiting_list;
int sptr=-1;struct dum *top;
int qptr=-1;struct samp *front;
int min_vacant=0;
class car
{
    char availability,car_name[MAX],car_num[MAX],car_type[MAX],fuel_type[MAX];
    int rent,serial_num;
    car *link;
    public:
    car()
    {
        availability='Y';
    }
    void get_info()
    {
        cout<<"\n\tFeed the Car's name :";
        cin>>car_name;
        cout<<"\n\tFeed the Car's Registration Number :";
        cin>>car_num;
        cout<<"\n\tFeed the Car's Type :";
        cin>>car_type;
        cout<<"\n\tFeed Car's Fuel Type :";
        cin>>fuel_type;
        cout<<"\n\tFeed Car's Rent per day :";
        cin>>rent;
    }
    void put_info()
    {
        cout<<"\n\tCar's name :"<<car_name;
        cout<<"\n\tCar's Registration Number :"<<car_num;
        cout<<"\n\tCar's Type :"<<car_type;
        cout<<"\n\tCar's Fuel Type :"<<fuel_type;
        cout<<"\n\tCar's Rent per day :"<<rent;   
    }
    void addcar(car &vehicle)
    {
        fstream myfile;
        myfile.open("Car Details.DAT",ios::out|ios::app|ios::binary);
        if(myfile.is_open())
        {
            myfile.write((char*)&vehicle,sizeof(car));
        }
        else
        {
            cout<<"\n\tError !!";
        }
        myfile.close();
    }
    car* cll(car &vehicle)
    {
        int count=0;
        fstream myfile;
        myfile.open("Car Details.DAT",ios::in|ios::binary);
        car *first,*ptr,*tptr;
        if(myfile.is_open())
        {
            myfile.read((char*)&vehicle,sizeof(car));
            while(!myfile.eof())
            {
                if(count==0)
                {
                    first=new car;
                    tptr=first;
                    count++;
                }
                else
                {
                    ptr=new car;
                    tptr->link=ptr;
                    tptr=ptr;
                }
                strcpy(tptr->car_name,vehicle.car_name);
                strcpy(tptr->car_num,vehicle.car_num);
                strcpy(tptr->car_type,vehicle.car_type);
                strcpy(tptr->fuel_type,vehicle.fuel_type);
                tptr->availability=vehicle.availability;
                tptr->rent=vehicle.rent;
                if(tptr->availability=='Y')
                {
                    min_vacant=1;
                }
                myfile.read((char*)&vehicle,sizeof(car));
            }
            tptr->link=first;
            myfile.close();
            return first;
        }
        else
        {
            min_vacant=-1;
            return 0;
        }
    } 
    void change_status(char* reg_num,car *tptr)
    {
    	int count=0;
        fstream myfile,tempfile;
        myfile.open("Car Details.DAT",ios::in|ios::binary|ios::app|ios::out);
        tempfile.open("Temp.DAT",ios::binary|ios::out|ios::in|ios::app);
        car vehicle;
        if(myfile.is_open())
        {
            myfile.read((char*)&vehicle,sizeof(car));
            while(!myfile.eof())
            {
                if(strcmp(vehicle.car_num,reg_num)==0)
                {
                    customer ptr;
                    cout<<"\n\tEnter Your Name :";
                    scanf("%s",ptr.name);
                    cin.ignore();
                    cout<<"\n\tEnter Your Mobile Number :";
                    cin>>ptr.mobile;
                    cout<<"\n\tEnter Your Address :";
                    cin>>ptr.address;
                    cout<<"\n\tEnter Your Age :";
                    cin>>ptr.age;
                    if(ptr.age<18)
                    {
                        cout<<"Sorry, you are not eligible";
                    }
                    else
                    {
                        tptr->availability='N';
                        cout<<"\n\tEnter Your License Number :";
                        cin>>ptr.license;
                        vehicle.availability='N';
                        tempfile.write((char*)&vehicle,sizeof(car));
                        strcpy(ptr.car,vehicle.car_name);
                        strcpy(ptr.car_no,vehicle.car_num);
                        fstream bookingfile;
                        bookingfile.open("Booking Details.DAT",ios::binary|ios::out|ios::app);
                            if(bookingfile.is_open())
                            {
                                bookingfile.write((char*)&ptr,sizeof(struct dum));
                            }
                            else
                            {
                                cout<<"Error in Booking File";
                            }   
                            min_vacant=0;
                    }
                }
                else
                {
                    if(vehicle.availability=='Y')
                    {   
                        min_vacant=1;
                    }
                    tempfile.write((char*)&vehicle,sizeof(car));	
				}
                myfile.read((char*)&vehicle,sizeof(car));
            }
            myfile.close();
            tempfile.close();
            remove("Car Details.DAT");
            rename("Temp.DAT","Car Details.DAT");
        }
        else
        {
            cout<<"\n\tError !!";
        }
    }
    void return_car()
    {
        int check=1;
        char renter_name[MAX],renter_mobile[MAX],car_rented[MAX],car_reg[MAX];
        fstream myfile;
        customer x;
        myfile.open("Booking Details.DAT",ios::binary|ios::in);
        if(myfile.is_open())
        {
            cout<<"\n\tEnter your name :";
            scanf("%s",renter_name);
            cout<<"\n\tEnter your mobile :";
            cin.ignore();
            cin>>renter_mobile;
            myfile.read((char*)&x,sizeof(struct dum));
            while(!myfile.eof())
            {
                if(strcmp(renter_mobile,x.mobile)==0)
                {
                    strcpy(car_rented,x.car);
                    strcpy(car_reg,x.car_no);
                    return_status(x.car_no,x.car);
                    break;
                }
                myfile.read((char*)&x,sizeof(struct dum));
            }
        }
        else
        {
            cout<<"\n\t\tReturn details doesn't exists !!";
            return;
        }
    }
    void return_status(char* reg_num,char* car_name)
    {
        int check=0;
        fstream myfile,tempfile;
        myfile.open("Car Details.DAT",ios::in|ios::binary|ios::app|ios::out);
        tempfile.open("Temp.DAT",ios::binary|ios::out|ios::in|ios::app);
        car vehicle;
        if(myfile.is_open())
        {
            myfile.read((char*)&vehicle,sizeof(car));
            while(!myfile.eof())
            {
                if((strcmp(vehicle.car_num,reg_num)==0)&&(strcmp(vehicle.car_name,car_name)==0))
                {
                    vehicle.availability='Y';
                    check=1;
                }
                tempfile.write((char*)&vehicle,sizeof(car));	
                myfile.read((char*)&vehicle,sizeof(car));
            }
            myfile.close();
            tempfile.close();
            remove("Car Details.DAT");
            rename("Temp.DAT","Car Details.DAT");
            if(check==1)
            {
                this->supply_queue(reg_num,car_name);
            }
        }
        else
        {
            cout<<"\n\tError !!";
        }
    }
    void supply_queue(char *reg_num,char *car_name)
    {
        int flag=0;
        //cout<<"\n\t\tName :"<<front->name<<"\n\t\tMobile :"<<front->mobile<<"\n\t\tAddress :"<<front->address<<"\n\t\tAge:"<<front->age<<endl;
        waiting_list x;
        fstream myfile,temp;
        myfile.open("Waiting List.DAT",ios::binary|ios::in);
        temp.open("Temporary.DAT",ios::binary|ios::out|ios::in|ios::app);
        if(myfile.is_open())
        {
            struct samp *removal=front;
            front=front->link;
            myfile.read((char*)&x,sizeof(struct samp));
            myfile.read((char*)&x,sizeof(struct samp));
            while(!myfile.eof())
            {
                flag=1;
                temp.write((char*)&x,sizeof(struct samp));
                myfile.read((char*)&x,sizeof(struct samp));
            }
            if(flag==0)
            {
                temp.close();
                temp.open("Temporary.DAT",ios::binary|ios::out);
            }
            myfile.close();
            temp.close();
            remove("Waiting List.DAT");
            rename("Temporary.DAT","Waiting List.DAT");
            this->change_status_queue(reg_num,car_name,removal);
        }
        else
        {
            cout<<"\n\t\tWaiting List info doesn't exists !!";
        }
    }
    
    
    
    //Need to be done .....
    
    
    
    void change_status_queue(char *reg_num,char *car_name,struct samp *ptr)
    {
        customer n;
        strcpy(n.name,ptr->name);
        strcpy(n.mobile,ptr->mobile);
        strcpy(n.address,ptr->address);
        strcpy(n.license,ptr->license);
        strcpy(n.car,car_name);
        strcpy(n.car_no,reg_num);
        n.age=ptr->age;
        fstream myfile,tempfile;
        myfile.open("Car Details.DAT",ios::in|ios::binary|ios::app|ios::out);
        tempfile.open("Temp.DAT",ios::binary|ios::out|ios::in|ios::app);
        car vehicle;
        if(myfile.is_open())
        {
            myfile.read((char*)&vehicle,sizeof(car));
            while(!myfile.eof())
            {
                if(strcmp(vehicle.car_num,reg_num)==0)
                {
                    //cout<<"\n\t\tFounded";
                    vehicle.availability='N';
                    tempfile.write((char*)&vehicle,sizeof(car));
                    fstream bookingfile;
                    bookingfile.open("Booking Details.DAT",ios::binary|ios::out|ios::app);
                        if(bookingfile.is_open())
                        {
                            bookingfile.write((char*)&n,sizeof(struct dum));
                        }
                        else
                        {
                            cout<<"Error in Booking File";
                        }
                }
                else
                tempfile.write((char*)&vehicle,sizeof(car));
                myfile.read((char*)&vehicle,sizeof(car));
            }
            myfile.close();
            tempfile.close();
            remove("Car Details.DAT");
            rename("Temp.DAT","Car Details.DAT");
        }
        else
        cout<<"\n\t\t325 Error !!";
    }
    void display_cll(car *ptr)
    {
        int check=1;
        car *tptr=ptr,*fptr=ptr;
        if(min_vacant==1)
        {
            while(check==1)
            {
                cout<<"\n\tCar's name :"<<tptr->car_name;
                cout<<"\n\tCar's Registration Number :"<<tptr->car_num;
                cout<<"\n\tCar's Type :"<<tptr->car_type;
                cout<<"\n\tCar's Fuel Type :"<<tptr->fuel_type;
                cout<<"\n\tCar's Rent per day :"<<tptr->rent;
                cout<<"\n\tCar's Status :"<<tptr->availability;
                cout<<"\n\t\t(Next Car = 1 / Book this car =2)";
                cin.ignore();
                cin>>check;
                if(check==2)
                {
                    if(tptr->availability=='N')
                        cout<<"\n\t\tThis car is engaged";
                    else
                        this->change_status(tptr->car_num,tptr);
                    break;
    		    }
    		    else if((check<1)&&(check>2))
    		    {
    		        cout<<"\n\t\t\tInvalid choice...";
    		    }
    		   tptr=tptr->link;
            }
        }
        else if(min_vacant==-1)
        printf("\n\t\tCar details doesn't exists !!");
        else
        {
            int ch;
            cout<<"\n\tAll of our vehicles are engaged...!!\n\tKindly register for booking..\n\t*T&C apply";
            cout<<"\n\n\t\tEnter Your Choice :\n\t\t\tFor booking enter 1\n\t\t\tFor Exit enter 2";
            cin>>ch;
            if(ch==1)
            {
                waiting_list x;
                cout<<"\n\tEnter Your Name :";
                cin>>x.name;
                cout<<"\n\tEnter Your Mobile Number :";
                cin>>x.mobile;
                cout<<"\n\tEnter Your Address :";
                cin>>x.address;
                cout<<"\n\tEnter Your age :";
                cin>>x.age;
                if(x.age<18)
                {
                    cout<<"\n\t\t\tSorry, you are not eligible !!";
                }
                else
                {
                    cout<<"\n\tEnter Your License Number :";
                    cin>>x.license;
                    fstream myfile;
                    myfile.open("Waiting List.DAT",ios::binary|ios::in|ios::app);
                    if(myfile.is_open())
                    {
                        myfile.write((char*)&x,sizeof(waiting_list));
                        myfile.close();
                    }
                    else
                    cout<<"\n\tError in opening waiting list";
                }  
            
            }
            else if(ch==2)
            {
                cout<<"\n\tThanks for your visit";
            }
            else
               cout<<"\n\t\t\tInvalid Choice...";
        }
    }
};
void booking_history()
{
    customer x;
    struct dum *tptr,*ptr;
    int count=0;
    fstream myfile;
    myfile.open("Booking Details.DAT",ios::binary|ios::in);
    if(myfile.is_open())
    {
        myfile.read((char*)&x,sizeof(struct dum));
        while(!myfile.eof())
        {
            if(count==0)
            {
                tptr=new struct dum;
                tptr->link=NULL;
                ++count;
            }
            else
            {
                ptr=new struct dum;
                ptr->link=tptr;
                tptr=ptr;
            }
            strcpy(tptr->name,x.name);
            strcpy(tptr->mobile,x.mobile);
            strcpy(tptr->address,x.address);
            strcpy(tptr->license,x.license);
            strcpy(tptr->car,x.car);
            strcpy(tptr->car_no,x.car_no);
            tptr->age=x.age;
            myfile.read((char*)&x,sizeof(struct dum));
        }
        top=tptr;
        myfile.close();
    }
    else
    {
        cout<<"\n\t\tNo booking history exists !!";
        return;
    }
    struct dum *temp;
    temp=top;
    do
    {
        cout<<"\n\t\tName :"<<temp->name;
        cout<<"\n\t\tMobile :"<<temp->mobile;
        cout<<"\n\t\tCar Rented :"<<temp->car;
        cout<<"\n\t\tCar's Registration Number :"<<temp->car_no;
        cout<<"\n\t\tAddress :"<<temp->address<<endl;
        temp=temp->link;
    }while(temp!=NULL);
}
void waiting_list_info()
{
    int flag=0;
    waiting_list x;
    struct samp *tptr,*ptr;
    int count=0;
    fstream myfile;
    myfile.open("Waiting List.DAT",ios::binary|ios::in);
    if(myfile.is_open())
    {
        myfile.read((char*)&x,sizeof(struct samp));
        while(!myfile.eof())
        {
            flag=1;
            if(count==0)
                {
                    front=new struct samp;
                    tptr=front;
                    count++;
                }
            else
                {
                    ptr=new struct samp;
                    ptr->link=NULL;
                    tptr->link=ptr;
                    tptr=ptr;
                }
            strcpy(tptr->name,x.name);
            strcpy(tptr->mobile,x.mobile);
            strcpy(tptr->address,x.address);
            tptr->age=x.age;
            myfile.read((char*)&x,sizeof(struct samp));
        }
        myfile.close();
        if(flag==0)
        {
        cout<<"\n\tNo info exists !!";
        return;
        }
    }
    else
    {
        cout<<"\n\t\tWaiting list data doesn't exists...";
        return;
    }
    struct samp *temp;
    temp=front;
    do
    {
        cout<<"\n\t\tName :"<<temp->name<<"\n\t\tMobile :"<<temp->mobile<<"\n\t\tAddress :"<<temp->address<<"\n\t\tAge:"<<temp->age<<endl;
        temp=temp->link;
    }while(temp!=NULL);
}
int main()
{
    int opinion;
    do
    {
        cout<<"\n\tEnter Your Choice :\n\t\t1)Add a Car\n\t\t2)Menu of Cars\n\t\t3)Booking History\n\t\t4)Waiting List\n\t\t5)Return Car\n\t\t";
        cin>>opinion;
        switch(opinion)
        {
            case 1:
            {
                car a;
                a.get_info();
                a.addcar(a);
                break;
            }
            case 2:
            {
                car a;
                car *ptr=a.cll(a);
                a.display_cll(ptr);
                break;
            }
            case 3:
            {
                booking_history();
                break;
            }
            case 4:
            {
                waiting_list_info();
                break;
            }
            case 5:
            {
                car a;
                a.return_car();
                break;
            }
            default:
            {
                cout<<"\n\t\tInvalid Choice !!";
            }
        }
    }while(opinion!=-1);
    return 0;
}