#include <iostream>
using namespace std;
#define COUNT 6

struct node {
int data;
node* trai;
node* phai;
node* cha;
bool mau; //0:den  1:do
};

node* maxValuenode(node* x) //tim node co gia tri lon nhat
{
    node* node_max = x;
    while (node_max->phai!=NULL)
        node_max = node_max->phai;
    return node_max;
}
node* sibling(node* x) //tim node anh em
{
    if (x->cha==NULL)
        return NULL;
    if (x->cha->trai==x)
        return x->cha->phai;
    return x->cha->trai;
}
bool hasRedChild(node* x) //kiem tra node con cua x co phai mau do khong
{
    if (x->phai != NULL)
        if (x->phai->mau == 1)
            return true;
    if (x->trai != NULL)
        if (x->trai->mau == 1)
            return true;
    return false;
}

void doi_mau(node* x1, node* x2)
{
    bool temp;
    temp = x1->mau;
    x1->mau = x2->mau;
    x2->mau = temp;
}

void doi_data(node* u, node* v)
{
    int temp;
    temp = u->data;
    u->data = v->data;
    v->data = temp;
}

string getmau(node* x)
{
    if (x->mau==1)
        return "(R)";
    return "(B)";
}

void print2DUtil(node* x, int space)
{
    if (x==NULL)
        return;
    space = space + COUNT;
    print2DUtil(x->phai, space);
    cout<<endl;
    for(int i=COUNT; i<space; i++)
        cout<<" ";
    cout<<x->data<<getmau(x)<<"\n";
    print2DUtil(x->trai, space);
}
void print2D(node* x)
{
    print2DUtil(x, 0);
}

struct RBTree {
node* Root;
bool ll=false;
bool rr=false;
bool lr=false;
bool rl=false;
RBTree() {Root=NULL;}

node* xoay_phai(node* x)
{
    node* k = x->trai;
    x->trai = k->phai;
    if (k->phai != NULL)
        k->phai->cha = x;
    k->cha = x->cha;
    if (x->cha == NULL)
        Root = k;
    else if (x->cha->trai == x)
        x->cha->trai = k;
    else
        x->cha->phai = k;
    k->phai = x;
    x->cha = k;
    return k;
}
node* xoay_trai(node* x)
{
    node* k = x->phai;
    x->phai = k->trai;
    if (k->trai != NULL)
        k->trai->cha = x;
    k->cha = x->cha;
    if (x->cha == NULL)
        Root = k;
    else if (x->cha->trai == x)
        x->cha->trai = k;
    else
        x->cha->phai = k;
    k->trai = x;
    x->cha = k;
    return k;
}

node* them_node(node* x, int key)
{
    bool xung_dot_do=false;
    if(x==NULL)
    {return new node{key, NULL, NULL, NULL, 1};} //luon them node mau do
    else
    {
        if(key > x->data)
        {
            x->phai=them_node(x->phai,key);
            x->phai->cha=x;
            if (Root != x)
            {
                if(x->mau == x->phai->mau == 1) xung_dot_do=true;
            }
        }
        else
        {
            x->trai = them_node(x->trai, key);
            x->trai->cha = x;
            if (Root != x)
            {
                if(x->mau == x->trai->mau == 1) xung_dot_do=true;
            }
        }
    }
    //xu li xung dot do
    //TH1: cha mau do, chu mau den
    if (ll)
    {
        x=xoay_phai(x);
        x->mau=0;
        x->phai->mau=1;
        ll = false;
    }
    else if (rr)
    {
        x = xoay_trai(x);
        x->mau=0;
        x->trai->mau=1;
        rr = false;
    }
    else if (lr)
    {
        x->trai=xoay_trai(x->trai);
        x->trai->cha = x;
        x = xoay_phai(x);
        x->mau = 0;
        x->phai->mau = 1;
        lr = false;
    }
    else if (rl)
    {
        x->phai = xoay_phai(x->phai);
        x->phai->cha = x;
        x = xoay_trai(x);
        x->mau = 0;
        x->trai->mau = 1;
        rl = false;
    }
    //TH2: cha mau do, chu mau do
    if(xung_dot_do)
    {
        if(x->cha->phai==x)
        {
            if(x->cha->trai==NULL || x->cha->trai->mau==0) // cha mau do, chu mau den -> TH1
            {
                if (x->trai!=NULL && x->trai->mau==1)
                    rl = true;
                if (x->phai!=NULL && x->phai->mau==1)
                    rr = true;
            }
            else //cha mau do, chu mau do
            {
                x->mau = x->cha->trai->mau = 0;
                if (x!=Root)
                    x->cha->mau = 1;
            }
        }
        else
        {
            if (x->cha->phai==NULL || x->cha->phai->mau==0)
            {
                if (x->trai!=NULL && x->trai->mau==1)
                    ll = true;
                if (x->phai!=NULL && x->phai->mau==1)
                    lr = true;
            }
            else
            {
                x->mau = x->cha->phai->mau = 0;
                if (x != Root)
                    x->cha->mau = 1;
            }
        }
        xung_dot_do = false;
    }
    return x;
}
void insert(int key)
{
    if(Root==NULL)
    {Root=new node{key, NULL, NULL, NULL, 0};} //node Root mau den
    else
    {
        Root=them_node(Root, key);
        if (Root->mau==1) Root->mau=0; //neu Root bi doi thanh mau do -> doi lai thanh mau den
    }
}
void fixDoubleBlack(node* x)
{
    if (x==Root)
        return;
    node* sib = sibling(x);
    node* par = x->cha;
    if (sib == NULL)
        fixDoubleBlack(par);
    else
    {
        if (sib->mau == 1)
        {
            par->mau = 1;
            sib->mau = 0;
            if (sib->cha->trai == sib) par=xoay_phai(par);
            else par=xoay_trai(par);
            fixDoubleBlack(x);
        }
        else
        {
            if (hasRedChild(sib))
            {
                if (sib->cha->trai == sib)
                {
                    if (sib->trai != NULL && sib->trai->mau)
                    {
                        sib->trai->mau = sib->mau;
                        sib->mau = par->mau;
                        par->mau = 0;
                        par = xoay_phai(par);
                    }
                    else
                    {
                        sib->phai->mau = par->mau;
                        par->mau = 0;
                        sib = xoay_trai(sib);
                        par = xoay_phai(par);
                    }
                }
                else
                {
                    if (sib->phai != NULL && sib->phai->mau)
                    {
                        sib->phai->mau = sib->mau;
                        sib->mau = par->mau;
                        par->mau = 0;
                        par = xoay_trai(par);
                    }
                    else
                    {
                        sib->trai->mau = par->mau;
                        par->mau = 0;
                        sib = xoay_phai(sib);
                        par = xoay_trai(par);
                    }
                }
            }
            else
            {
                sib->mau = 1;
                if (par->mau == 0) fixDoubleBlack(par);
                else par->mau = 0;
            }
        }
    }
}
void xoa_node(node* vDelete) //vDelete: node bi xoa; uReplace: node thay the
{
    node* uReplace;
    if ((vDelete->trai != NULL) && (vDelete->phai != NULL))
        uReplace = maxValuenode(vDelete->trai);
    else if (vDelete->trai != NULL)
        uReplace = vDelete->trai;
    else if (vDelete->phai != NULL)
        uReplace = vDelete->phai;
    else
        uReplace = NULL;
    bool uvBlack = ((uReplace == NULL) || (uReplace->mau == 0)) && (vDelete->mau == 0);
    node* par = vDelete->cha;
    node* sib = sibling(vDelete);
    if (uReplace == NULL)
    {
        if (vDelete == Root) Root = NULL;
        else
        {
            if (uvBlack) fixDoubleBlack(vDelete);
            else if(sib != NULL) sib->mau = 1;
            if (vDelete->cha->trai == vDelete) par->trai = NULL;
            else par->phai = NULL;
        }
        delete vDelete;
        return;
    }
    if (vDelete->trai == NULL || vDelete->phai == NULL)
    {
        if (vDelete == Root)
        {
            vDelete->data = uReplace->data;
            vDelete->trai = vDelete->phai = NULL;
            delete uReplace;
        }
        else
        {
            if (vDelete->cha->trai == vDelete) par->trai = uReplace;
            else par->phai = uReplace;
            delete vDelete;
            uReplace->cha = par;
            if (uvBlack) fixDoubleBlack(uReplace);
            else uReplace->mau = 0;
        }
        return;
    }
    doi_data(uReplace, vDelete);
    xoa_node(uReplace);
}
node* search(int val)
{
    node* temp = Root;
    while (temp != NULL)
    {
        if (val < temp->data)
        {
            if (temp->trai == NULL) return NULL;
            else
                temp = temp->trai;
        }
        else if (val == temp->data) break;
        else
        {
            if (temp->phai == NULL) return NULL;
            else
            temp = temp->phai;
        }
    }
    return temp;
}
void Delete(int x)
{
    node* vDelete = search(x);
    if (vDelete == NULL) cout << "\nKhong tim thay node can xoa\n";
    else xoa_node(vDelete);
    return;
}
};
node* timkiem(int x, RBTree A)
{
    node* temp = A.Root;
    while (temp != NULL)
    {
        if (x<temp->data)
        {
            if (temp->trai==NULL) return NULL;
            else temp=temp->trai;
        }
        else if (x==temp->data) break;
        else
        {
            if (temp->phai==NULL) return NULL;
            else temp=temp->phai;
        }
    }
    return temp;
}

bool kiemtrarong(RBTree A)
{
    if(A.Root!=NULL ) return false;
    return true;
}
//duyet cay
void PreOrder(node* x)
{
    if(x!=NULL)
    {
        cout<<x->data<<" ";
        PreOrder(x->trai);
        PreOrder(x->phai);
    }
}
void InOrder(node* x)
{
    if(x!=NULL)
    {
        InOrder(x->trai);
        cout<<x->data<<" ";
        InOrder(x->phai);
    }
}
void PostOrder(node* x)
{
    if(x!=NULL)
    {
        PostOrder(x->trai);
        PostOrder(x->phai);
        cout<<x->data<<" ";
    }
}

int main()
{
    int x, y; RBTree A;
do{
    cout<<"Chon: \n\t1.Khoi tao cay \n\t2.Kiem tra rong \n\t3.Them node \n\t4.Xoa node \n\t5.Tim kiem node \n\t6.Duyet cay \n\t7.Huy cay \n\t8.Hoan tat"<<endl;
    cout<<"Lua chon cua ban la: "; cin>>x;
    while(x!=1 && x!=2 && x!=3 && x!=4 && x!=5 && x!=6 && x!=7 && x!=8)
    {
        cout<<"Lua chon khong hop le. Chon lai: ";cin>>x;
    }
    switch(x)
    {
    case 1:
        {
            cout << "\n---------------------------\n";
            y=1;
            cout<<"Da khoi tao cay thanh cong!";
            cout << "\n---------------------------\n";
            break;
        }
    case 2:
        {
            cout << "\n---------------------------\n";
            if(y!=1) cout<<"Chua khoi tao cay!";
            else
            {
                if(kiemtrarong(A)==true) cout<<"Cay rong";
                else cout<<"Khong rong";
            }
            cout << "\n---------------------------\n";
            break;
        }
    case 3:
        {
            if(y!=1) cout<<"\n---------------------------\nChua khoi tao cay!";
            else {
            int m; cout<<"Gia tri node muon them: "; cin>>m;
            cout<<"\n---------------------------\n";
            A.insert(m);
            print2D(A.Root);
            }
            cout <<"\n---------------------------\n";
            break;
        }
    case 4:
        {
            if(y!=1) cout<<"\n---------------------------\nChua khoi tao cay!";
            else {
            int m; cout<<"Gia tri node muon xoa: "; cin>>m;
            cout<<"\n---------------------------\n";
            A.Delete(m);
            print2D(A.Root);
            }
            cout << "\n---------------------------\n";
            break;
        }
    case 5:
        {

            if(y!=1) cout<<"\n---------------------------\nChua khoi tao cay!";
            else {
            int m; cout<<"Gia tri node muon tim: "; cin>>m;
            cout << "\n---------------------------\n";
            cout<<timkiem(m,A);
            }
            cout << "\n---------------------------\n";
            break;
        }
    case 6:
        {
            if(y!=1) cout<<"\n---------------------------\nChua khoi tao cay!";
            else {
            cout<<"Duyet theo thu tu: \n\t1.PreOrder \n\t2.InOrder \n\t3.PostOrder"<<endl;
            cout<<"Lua chon cua ban la: ";
            int q; cin>>q;
            while(q!=1 && q!=2 && q!=3)
            {
                cout<<"Lua chon khong hop le. Chon lai: ";cin>>q;
            }
            cout << "\n---------------------------\n";
            switch(q)
            {
                case 1:
                    {
                        PreOrder(A.Root);
                        break;
                    }
                case 2:
                    {
                        InOrder(A.Root);
                        break;
                    }
                case 3:
                    {
                        PostOrder(A.Root);
                        break;
                    }
            }}
            cout << "\n---------------------------\n";
            break;
        }
    case 7:
        {
            cout << "\n---------------------------\n";
            if(y!=1) cout<<"Chua khoi tao cay!";
            else {
            while(kiemtrarong(A)==false)
            {
                A.Delete(A.Root->data);
            }
            y=0;
            cout<<"Da huy cay!";
            }
            cout << "\n---------------------------\n";
            break;
        }
    case 8: exit(1);
    }
}while(x!=8);
}
