#include <iostream>
#include <fstream>
#include <limits.h>
 

using namespace std;
// Implementar un max_heap
template<class T>

class Heap
{
    private: 
      T *   m_Heap;
      int   m_size;
      int   m_capacity;

    private:
    int LS(int i){ return 2*i+1;}; // O(1)
    int RS(int i){return 2*i+2;}; // O(1)
    int Get_Father(int i){return (i-1)/2;} // O(1)
    
    void HeapFy(int i)
    {
        int m = i;
        int PI = LS(i);
        int PD = RS(i);
        if((PD < m_size) && (m_Heap[m] > m_Heap[PD])) m = PD;
        if((PI < m_size) && (m_Heap[m] > m_Heap[PI])) m = PI;
        if(m != i) {
            std::swap(m_Heap[m] , m_Heap[i]);
            HeapFy(m);
        }else return;
    }

    public:
    Heap(int n) 
    {
        m_Heap = new T[n];
        m_size = 0;
        m_capacity = n;
    }

    T Get_Min() // O(1)
    {
        return m_Heap[0];
    }

    void MakeHeap(int * vec, int  n) // O(nlogn)
    {
        // copia los datos de vec al heap o(n)
        for( int i = 0; i<n; i++)
          m_Heap[i]=vec[i];
        m_size = n;
        // tranforma los datos en un heap
        for(int i = (n/2+1) ; i >= 0; i--)
            HeapFy(i);
        
    }

    void Delete(int i) // O(log(n))
    {
        Decrease_Key(i, INT_MIN ); // O(log(n))
        Extract_Min(); // O(log(n))
    }

    void Decrease_Key(int i, T value) // O(log(n))
    {
        m_Heap[i] = value;
        while(i > 0 && m_Heap[Get_Father(i)] > m_Heap[i])
        {
            std::swap(m_Heap[i] , m_Heap[Get_Father(i)]);
            i = Get_Father(i);
        }
    }

    void Extract_Min() //O(log(n))
    {
        if(m_size < 1) return;
        int min = m_Heap[0];
        m_Heap[0] = m_Heap[m_size-1];
        m_size--;
        HeapFy(0);
    }

    void Insert(T d)// O(log(n))
    {
        if(m_size < m_capacity)
        {
          m_Heap[m_size]=d;
          m_size++;
          Decrease_Key(m_size,d);
        } 
    }

    // Para ordenar debemos crear un max_heap
    int  *  HeapSort(int  * v , int  n) //O(nlogn)
    {
        MakeHeap(v,n); // construye el heap O(nlogn)
        for(int i=0; i<n;i++)
        {
            swap(m_Heap[0],m_Heap[m_size]);
            m_size--; 
            HeapFy(0);
        }
        for( int i = 0; i<n; i++)
          v[i] = m_Heap[i];
        m_size=n-1; 
        
       return v;
    }

    void print()
    {
        for(int i = 0 ; i < m_size ; i++)
        {
            std::cout<<m_Heap[i]<<" ";
        }
    }

    
      
        void Show_Dot(int i, ostream & o)
        {
            if(m_size <= i) return;
            if( LS(i) < m_size) o<<"\t"<<i<<" -> "<<LS(i)<<"\t[label= 0]"<<";\n";
            if( RS(i) < m_size) o<<"\t"<<i<<" -> "<<RS(i)<<"\t[label= 1]"<<";\n";
            Show_Dot(LS(i), o);
            Show_Dot(RS(i), o);
        }

        void Show_Dot(ostream & o)
        {
            o<< "digraph B {"<<endl;
            o<< "node [shape=record];"<<endl;
            o<< "\t"<<0<< "[style=filled];" << endl;
            for (int i = 0; i < m_size; i++)
                o << "    " << i <<  " [label= \" " << i << " | " <<  m_Heap[i] <<  " \" ];" << endl;
            Show_Dot(0, o); 
            o<< "}"<<endl;
        }



    
};
int main() 
{
    Heap<int> A(5);
    for(int i = 1; i< 5; i ++) 
    {
        int a = rand()%1000;
        A.Insert(a);
    } 

    ofstream archivo;
    archivo.open("D:\\Graphviz\\bin\\binomial.dot");
    A.Show_Dot(archivo);
    archivo.close();
    system("dot -Tpng D:\\Graphviz\\bin\\binomial.dot -o D:\\Graphviz\\bin\\binomial.png ");
    system("D:\\Graphviz\\bin\\binomial.png" );

    int x;
    cout<<"Eliminando "; cin>>x;
    while(x!=-1)
    { 
        A.Delete(x);
        archivo.open("D:\\Graphviz\\bin\\binomial.dot");
        A.Show_Dot(archivo);
        archivo.close();
        A.print();
        system("dot -Tpng D:\\Graphviz\\bin\\binomial.dot -o D:\\Graphviz\\bin\\binomial.png ");
        system("D:\\Graphviz\\bin\\binomial.png" );
        cout<<"Eliminando "; cin>>x;
    }
    
    system("dot -Tpng D:\\Graphviz\\bin\\binomial.dot -o D:\\Graphviz\\bin\\binomial.png ");
    system("D:\\Graphviz\\bin\\binomial.png" );
    return 0;
}