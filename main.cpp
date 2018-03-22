#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
using namespace std;
template <typename T>
class Mat{
//因为是类模板，禁!止!不同类型的各种运算
public:
    //各种构造函数...
    Mat<T>(){
        cols = 0;
        rows = 0;
    }
    Mat<T>(int rows_,int cols_,vector<T> mat){
        rows = rows_;
        cols = cols_;

        for (int i=0;i<rows;i++){
            vector<T> t;
            for (int j=0;j<cols;j++){
                t.push_back(mat[i*cols+j]);
            }
            __Mat.push_back(t);
        }
    }
    Mat<T>(int rows_,int cols_){
        rows = rows_;
        cols = cols_;
        for (int i=0;i<rows;i++){
            vector <T> t;
            for (int j=0;j<cols;j++){
                t.push_back(0);
            }
            __Mat.push_back(t);
        }
    }
    Mat<T>(Mat<T> &tmp):__Mat(tmp.data()){
        cols = tmp.col();
        rows = tmp.row();
    }
private:
    //数据成员
    int cols,rows;
    vector < vector<T> > __Mat;
private:
    //getReNum用于获得逆序数
    int getReNum(vector<T> oneRow){
        typename vector<T>::size_type i(0),j;
        int sum(0);
        for (;i<oneRow.size();i++){
            for (j=0;j<i;j++){
                if (oneRow[j] > oneRow[i])
                    sum ++;
            }
        }
        return sum;
    }

public:
    //det返回行列式
    //algRemain返回余子式对应的矩阵
    //data返回__Mat
    //col返回cols
    //row返回rows
    //tMat返回转制矩阵
    //show输出矩阵

    T det(){
        if(this->cols == this->rows){
            //对一阶的特殊处理
            if (this->cols == 1){
                typename vector< vector<T> >::size_type i(0);
                typename vector<T>::size_type j(0);
                return __Mat[i][j];
            }
            T ans(0);
            vector<int> each;
            for (int i = 0;i<cols;i++){
                each.push_back(i);
            }
            //使用C++标准库中的next_permutation函数获得全排列
            do{
                if (getReNum(each)%2 == 0){//为偶排列
                    T t(1);
                    for (int i=0;i<cols;i++){
                        t *= __Mat[i][each[i]];
                    }
                    ans += t;
                }
                else{//为奇排列
                    T t(1);
                    for (int i=0;i<cols;i++){
                        t *= __Mat[i][each[i]];
                    }
                    ans -= t;
                }
            }while( std::next_permutation(&each[0],&each[cols]) );
            return ans;
        }
        else{
            cerr<<"Error:"<<__FILE__
               <<":line"<<__LINE__<<endl
              <<"     Complied on"<<__DATE__
             <<"at"<<__TIME__<<endl
            <<"      det operation  was"<<endl
            <<": The det didn't exist."<<endl;
            T t(0);
            return t;
        }
    }
    vector< vector<T> > data(){
        return __Mat;
    }
    Mat<T> algRemain(int r,int c){
         vector<T> tResult;
         typename vector< vector<T> >::size_type i(0);
         for (;i != __Mat.size();i++){
             if ( (int)i ==  r)
                 continue;
             vector<T> t = __Mat[i];
             typename vector<T>::size_type j(0);
             for (;j != __Mat.size();j++){
                 if ((int)j == c)
                     continue;
                 else
                    tResult.push_back(t[j]);
             }
         }
         Mat<T> ans(rows-1,cols-1,tResult);
         return ans;
    }
    int col(){
        return cols;
    }
    int row(){
        return rows;
    }
    Mat<T> tMat(){
        vector<T> tResult;
        typename vector< vector<T> >::size_type col_i(0);
        for (;(int)col_i<cols;col_i++){
            typename vector<T>::size_type row_j(0);
            for (;(int)row_j<rows;row_j++){
                tResult.push_back(__Mat[row_j][col_i]);
            }
        }
        Mat <T> result(cols,rows,tResult);
        return result;
    }
    void show(){
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //!!!!!!!!!!!!typename!!!!!!!!!!!
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        //使用下标访问
//        for (typename vector< vector<T> >::size_type i = 0;i != __Mat.size();i++){
//            vector <T> t = mat[i];
//            for (typename vector<T>::size_type j = 0;j != t.size();j++){
//                cout<<t[j]<<' ';
//            }
//            cout<<endl;
//        }
        //使用迭代器访问
        for (typename vector< vector<T> >::iterator rowIt = this->__Mat.begin();
             rowIt != __Mat.end(); rowIt++){
            vector <T> t = *rowIt;
            for (typename vector<T>::iterator colIt = t.begin();
                 colIt != t.end();colIt++){
                cout<<*colIt<<'\t';
            }
            cout<<endl;
        }

    }

};
//重载运算符
//只允许同类型的进行操作
//（反正不同类型是未定义的）

//重载==和!=
//判断两个col和row是否一致
template <typename T>
bool operator ==(Mat<T> a,Mat<T> b){
    if (a.col() != b.col()
            || a.row() != b.row())
        return false;
    else
        return true;
}
template <typename T>
bool operator !=(Mat<T> a,Mat<T> b){
    if (a == b)
        return false;
    else
        return true;
}

//重载+和-
//矩阵加减法
template <typename T>
Mat<T> operator +(Mat<T> a,Mat<T> b){
    if (a == b){
        vector <T> tResult;
        vector< vector<T> > tA(a.data()),tB(b.data());

        typename vector< vector<T> >::size_type i(0),j(0);
        for (;i<tA.size()&&j<tB.size();
             i++,j++){
            vector<T> tA_row = tA[i];
            vector<T> tB_row = tB[j];
            typename vector<T>::size_type ii(0),jj(0);
            for (;ii<tA_row.size()&&jj<tB_row.size();
                 ii++,jj++){
                tResult.push_back(tA_row[ii] + tB_row[jj]);
            }
        }
        Mat<T> result(a.row(),a.col(),tResult);
        return result;
    }
    else{
        cerr<<"Error:"<<__FILE__
           <<":line"<<__LINE__<<endl
          <<"     Complied on"<<__DATE__
         <<"at"<<__TIME__<<endl
        <<"      '+'operation  was"<<endl
        <<": The size cann't fit."<<endl;
        Mat<T> t(0,0,vector<T>(0));
        return t;
    }
}
template <typename T>
Mat<T> operator -(Mat<T> a,Mat<T> b){
    if (a == b){
        vector <T> tResult;
        vector< vector<T> > tA(a.data()),tB(b.data());

        typename vector< vector<T> >::size_type i(0),j(0);
        for (;i<tA.size()&&j<tB.size();
             i++,j++){
            vector<T> tA_row = tA[i];
            vector<T> tB_row = tB[j];
            typename vector<T>::size_type ii(0),jj(0);
            for (;ii<tA_row.size()&&jj<tB_row.size();
                 ii++,jj++){
                tResult.push_back(tA_row[ii] - tB_row[jj]);
            }
        }
        Mat<T> result(a.row(),a.col(),tResult);
        return result;
    }
    else{
        cerr<<"Error:"<<__FILE__
           <<":line"<<__LINE__<<endl
          <<"     Complied on"<<__DATE__
         <<"at"<<__TIME__<<endl
        <<"      '-'operation  was"<<endl
        <<": The size cann't fit."<<endl;
        Mat<T> t(0,0,vector<T>(0));
        return t;
    }
}

//重载*
//矩阵数乘和矩阵乘法
template <typename T>
Mat<T> operator *(T a, Mat<T> b){
    vector <T> tResult;
    vector< vector<T> > tB(b.data());

    typename vector< vector<T> >::size_type j(0);
    for (;j<tB.size();j++){
        vector<T> tB_row = tB[j];
        typename vector<T>::size_type jj(0);
        for (;jj<tB_row.size();jj++){
            tResult.push_back(a * tB_row[jj]);
        }
    }
    Mat<T> result(b.row(),b.col(),tResult);
    return result;
}
template <typename T>
Mat<T> operator *(Mat<T> a,Mat<T> b){
    if (a.col() == b.row()){
         vector<T> tResult;
         vector< vector<T> > tA(a.data()),tB(b.tMat().data());

         typename vector< vector<T> >::size_type i(0),j(0);

         for(;i != tA.size();i++){
             vector<T> tA_row = tA[i];
             for (j=0;j != tB.size();j++){
                 vector<T> tB_col = tB[j];

                 T t(0);
                 typename vector<T>::size_type idex=0;
                 for (;idex != tA_row.size();idex ++){
                     t += tA_row[idex]*tB_col[idex];
                 }
                 tResult.push_back(t);
             }
         }

         Mat<T> result(a.row(),b.col(),tResult);
         return result;
    }
    else{
        cerr<<"Error:"<<__FILE__
           <<":line"<<__LINE__<<endl
          <<"     Complied on"<<__DATE__
         <<"at"<<__TIME__<<endl
        <<"      '*'operation  was"<<endl
        <<": The size cann't fit."<<endl;
        Mat<T> t(0,0,vector<T>(0));
        return t;
    }
}



int main(){
    vector<int> test;
    for (int i=0;i<100;i++){
        test.push_back(i);
    }
    //测试各种构造函数
    Mat<int> a1;
    Mat<int> a2(1,2);
    Mat<int> a3(3,2,test);
    Mat<int> a4(a1);

    //测试+,-法
    Mat<int> a5(3,4,test);
    Mat<int> a6(3,4,test);
    Mat<int> a7;

    a7 = a5 + a6;
    cout<<"a7:a5+a6"<<endl;
    a7.show();
    cout<<endl;

    a7 = a5 - a6;
    cout<<"a7:a5-a6"<<endl;
    a7.show();
    cout<<endl;

    //测试*法
    Mat<int> a8(3,4,test);
    Mat<int> a9;
    a9 = 4*a8;
    cout<<"a9:4*a8"<<endl;
    a9.show();
    cout<<endl;

    Mat<int> a10(4,6,test);
    Mat<int> a11;
    a11 = a8*a10;

    cout<<"a8:"<<endl;
    a8.show();
    cout<<endl;

    cout<<"a10:"<<endl;
    a10.show();
    cout<<endl;

    cout<<"a11:a8*a10"<<endl;
    a11.show();
    cout<<endl;

    //测试转制
    cout<<"a11T"<<endl;
    a11.tMat().show();
    cout<<endl;

    //测试行列式
    vector<int> test2;
    for (int i=0;i<4;i++){
        test2.push_back(i);
    }
    for (int i=3;i<8;i++){
        test2.push_back(i);
    }
    Mat<int> a12(3,3,test2);
    cout<<"a12:"<<endl;
    a12.show();
    cout<<endl;
    cout<<"det(a12):"<<a12.det()<<endl;

    //测试余子式
    cout<<"a12 1,1 remain"<<endl;
    a12.algRemain(0,0).show();
    cout<<endl;


    return 0;
}

//丢弃版本v1.0
/*
#include <QCoreApplication>

#include <iostream>
#include <vector>
using namespace std;

template <typename T,int N>
class Mat{

public:
    Mat(){//默认构造函数
        rows = 0;
        cols = 0;
    }
    Mat(int rows_,int cols_,T mat_[][N]){ //使用二维数组构造
        rows=rows_;cols=cols_;
        for (int i=0;i<rows;i++){
            vector <T> t;
            for (int j=0;j<cols;j++){
                t.push_back(mat_[i][j]);
            }
            mat.push_back(t);
        }
    }
    Mat(int rows_,int cols_){
        rows = rows_;
        cols = cols_;
    }

private:
    int rows,cols;
    vector < vector<T> > mat;
//    template <typename tT>
//    int TYPE(tT a){
//        if (typeid(a) == )
//    }

public:
    vector < vector<T> > data(){
        return mat;
    }

    T Det(){

    }
    void show(){
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //!!!!!!!!!!!!typename!!!!!!!!!!!
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        //使用下标访问
//        for (typename vector< vector<T> >::size_type i = 0;i != mat.size();i++){
//            vector <T> t = mat[i];
//            for (typename vector<T>::size_type j = 0;j != t.size();j++){
//                cout<<t[j]<<' ';
//            }
//            cout<<endl;
//        }
        //使用迭代器访问
        for (typename vector< vector<T> >::iterator rowIt = this->mat.begin();
             rowIt != mat.end(); rowIt++){
            vector <T> t = *rowIt;
            for (typename vector<T>::iterator colIt = t.begin();
                 colIt != t.end();colIt++){
                cout<<*colIt<<' ';
            }
            cout<<endl;
        }

    }

    int row(){return rows;}
    int col(){return cols;}
    //重载==，Size是否相等
    template<typename T1,int n1>
    bool operator ==(Mat<T1,n1> a){
        if (a.col()!=cols||a.row()!=rows)
            return false;
        return true;
    }
};
//除行列式及赋值外，其他运算
//+,-,.*,*,*=,+=,-=,.*=
//注意判断col and row是否相同需重载 ==
//使用运算符重载实现
//.*使用^代替,.*=用^=代替

template<typename T1,typename T,int N>
Mat<T,N> operator +(Mat <T,N> a, Mat<T1,N> b){
    if (a == b){
        //所以执行+运算时，位于+左边的T数据类型优先级要高于右边的
        //前面define白写了
        T resultMat [a.row()][a.col()];
        //用下标访问更方便
        typename vector< vector<T> >::size_type i = 0;
        typename vector< vector<T1> >::size_type j = 0;
        for (;i<a.data().size()&&j<b.data().size();i++,j++){
            vector<T> ti = a.data()[i];
            vector<T1> tj = b.data()[j];

            typename vector<T>::size_type ii = 0;
            typename vector<T1>::size_type jj = 0;
            for (;ii<ti.size()&&jj<tj.size();ii++,jj++){
                 resultMat[i][ii] = ti[ii] + tj[jj];
            }
        }
        Mat<T, > result(a.row(),a.col(),resultMat);
        return result;
    }
    else{
        cerr<<"Error:"<<__FILE__
           <<":line"<<__LINE__<<endl
          <<"     Complied on"<<__DATE__
         <<"at"<<__TIME__<<endl
        <<"      '='operation  was"<<endl
        <<": The size cann't fit."<<endl;
    }
}


int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

    int num[3][3]={1,2,3,4,5,6,7,8,9};
    int num2[3][3]= {2,3,4,5,6,7,8,9,10};
    Mat<int,3> t(3,3,num);
    Mat<int,3> t1(3,3,num2);
    if(t == t1)
        cout<<"equal!"<<endl;
    t = t + t1;
    t.show();
    t1.show();
    return a.exec();
}
*/
