#include<iostream>
#include<list>
#include<fstream>
using namespace std;

template<class T>
class NodoB
{
    public:
       int                  m_Grado;
       T                    m_Dato;
       NodoB<T>  *          m_Padre;
       list< NodoB<T> * >   m_Sons;

       NodoB(T d)
       {
           m_Dato = d;
           m_Grado = 0;
           m_Padre = 0;
       } 
};

template<class T>
class Binomial_Heap
{
     private:
       list<NodoB<T>*>  m_Roots;
       NodoB<T>         *  m_pMin;
     private:
       void Compactar(); // O(log(n))
       void Unir(NodoB<T> * p ,NodoB<T> * q );  // O(1)
     public:
      Binomial_Heap(){ m_pMin = 0; };
      ~Binomial_Heap(){};
      void Insert(T d); // O(log(n))
      void Extrac_Min(); // O(log(n))
      void Deletes(NodoB<T> * e); // O(log(n))
      void Delete(T d); // O(log(n))
      void Decrease_Key(NodoB<T> *& e, T val); // O(log(n))
      NodoB<T> * GetMin();// O(1)
      void Show_Dot();
      void showList();
      void Graficar();
      void SubGraficar(list<NodoB<T>*> m_Sons, ofstream & archivo);
      NodoB<T>* BuscarNodosRaices(T d );
      NodoB<T>* BuscarNodosHijos( list<NodoB<T>*> ,T d );
      void Deletedo(T e);
      void DeleteHijos(T e , NodoB<T> * f);
      void DeleteRaices(T e);
      void Show_Sons_Dot(list< NodoB<T> * > , ostream &);
      /**************************************/
};


template<class T>
void Binomial_Heap<T>::Insert(T d) // O(log(n))
{
    NodoB<T> * pNew = new NodoB<T>(d);
    if (m_Roots.size() == 0)
        m_pMin = pNew;
    else
        m_pMin = pNew->m_Dato < m_pMin->m_Dato ? pNew : m_pMin;
        
    m_Roots.push_front(pNew);
    Compactar();
}

template<class T>
void Binomial_Heap<T>::SubGraficar(list<NodoB<T>*> m_Sons, ofstream & archivo){
    int aea = 0;
    for(auto its = m_Sons.begin(); its != m_Sons.end() ; its++){
        archivo << (*its)->m_Padre->m_Dato << "[ label =\"{{" << (*its)->m_Padre->m_Dato << "|" << (*its)->m_Padre->m_Grado << "}|{"; 
        for (int i = 0; i < (*its)->m_Padre->m_Sons.size()-1; i++) archivo << "<f" << i << ">"<< " | ";
        archivo << "<f" << (*its)->m_Padre->m_Sons.size()-1 << ">" << "}}\"];" << endl;
        archivo << ((*its)->m_Padre->m_Dato) << ":<f" << aea<< ">" << "-> "<<(*its)->m_Dato << "; "<< endl;
        aea++;
        if(!(*its)->m_Sons.empty()) SubGraficar((*its)->m_Sons, archivo);
    }
}


template<class T>
void Binomial_Heap<T>::Graficar(){
    ofstream archivo;
    archivo.open("D:\\Graphviz\\bin\\binomial.dot");
    archivo << "digraph D{" << endl;
    archivo << "rankdir=\"TB\"" << endl;
    for(auto it = m_Roots.begin(); it != m_Roots.end(); it++){
        archivo << "subgraph r" << (*it)->m_Dato << "{" << endl;
        archivo << "node [shape=Mrecord];" << endl;
           archivo << (*it)->m_Dato << ";" << endl;
        if(!(*it)->m_Sons.empty()) SubGraficar((*it)->m_Sons, archivo); 
        archivo << "}" << endl; 
    }
    archivo << "}" << endl;
    archivo.close();
    system("dot -Tpdf D:\\Graphviz\\bin\\binomial.dot -o D:\\Graphviz\\bin\\binomial.pdf");
    system("D:\\Graphviz\\bin\\binomial.pdf");
}

template<class T>
void Binomial_Heap<T>::Extrac_Min() // O(log(n))
{
    typename list<NodoB<T>*>::iterator it = m_pMin->m_Sons.begin();
    for(; it != m_pMin->m_Sons.end(); it++)
           m_Roots.push_back(*it);
    m_Roots.remove(m_pMin);

    Compactar();

    auto  it4 = m_Roots.begin();
    m_pMin = *it4;
    for( ; it4 != m_Roots.end() ; it4++){
        auto tmp = *(it4);
        if(m_pMin->m_Dato > tmp->m_Dato) {
            m_pMin = tmp;
        }
    }
    
}

template<class T>
void Binomial_Heap<T>::Decrease_Key(NodoB<T> *& e, T val) // O(log(n))
{
    e->m_Dato = val;
    while((e->m_Padre) && (e->m_Padre->m_Dato > e->m_Dato))
    {
        cout<<e->m_Dato<<" "<<(e->m_Padre)->m_Dato<<endl;
        swap(e->m_Dato,(e->m_Padre)->m_Dato);
        e = e->m_Padre;
    }
}

template<class T>
void Binomial_Heap<T>::Deletes(NodoB<T> * e) // O(log(n))
{
    
    Decrease_Key(e , m_pMin->m_Dato - 1);
    m_pMin = e;
    Extrac_Min();
    
}

template<class T>
void Binomial_Heap<T>::Delete(T e) // O(log(n))
{
    NodoB<T> * aux = BuscarNodosRaices(e);
    if(aux) Deletes(aux);
}

template<class T>
NodoB<T> * Binomial_Heap<T>::BuscarNodosRaices(T d) // O(1)
{
    for(auto it = m_Roots.begin(); it != m_Roots.end(); it++)
    {
        if((*it)->m_Dato == d) return (*it);
        if(!(*it)->m_Sons.empty() && (*it)->m_Dato < d)
        {
            NodoB<T>* p_aux = BuscarNodosHijos((*it)->m_Sons, d); 
            if(p_aux) return p_aux;
        } 
    }
    return nullptr;
}

template<class T>
NodoB<T> * Binomial_Heap<T>::BuscarNodosHijos(list<NodoB<T>*> m_Sons, T d) // O(1)
{
    typename list< NodoB< T>  * >::iterator it = m_Sons.begin();
    for(; it != m_Sons.end(); it++)
    {
        if((*it)->m_Dato == d) return (*it);
        if(!(*it)->m_Sons.empty() && (*it)->m_Dato < d) {
            NodoB<T>* p_aux = BuscarNodosHijos((*it)->m_Sons , d);
            if(p_aux) return p_aux;    
        }
    }
    return nullptr;
}


template<class T>
NodoB<T> * Binomial_Heap<T>::GetMin()// O(1)
{
    return m_pMin;
}

template<class T>
void Binomial_Heap<T>::Compactar() // O(log(n))
{
    if(m_Roots.size() == 1) return;
    typename list<NodoB<T>*>::iterator it1 = m_Roots.begin();
    typename list<NodoB<T>*>::iterator it2 = m_Roots.begin();
    typename list<NodoB<T>*>::iterator it3 = m_Roots.end();
    NodoB<T> * tmp, * tmp2;
    it3--;
    for(; it1!=m_Roots.end(); it1++)
    {
        tmp2 = *it2;
        tmp = *it1;
        if(tmp->m_Grado == tmp2->m_Grado && tmp!=tmp2){
            Unir(tmp, tmp2);
            it1=m_Roots.begin();
            it2=m_Roots.begin();
            it3=m_Roots.end();
            it3--;
            tmp=*it1;
            tmp2=*it2;
        }
        if(it2!=it3 && it3==it1){
            it2++;
            it1=m_Roots.begin();
        }
    }
}


template<class T>
void Binomial_Heap<T>::showList()
{
    typename list<NodoB<T>*>::iterator it = m_Roots.begin();
    while(it != m_Roots.end()) {
        cout << (*it)->m_Dato << "[" << (*it)->m_Grado << "]";
        cout << " -> ";
        it++;
    }
    cout << "END";
    cout << endl;
}


// O(1)
template<class T>
void Binomial_Heap<T>::Unir(NodoB<T> * p ,NodoB<T> * q ) // O(1)
{
    if(p->m_Dato < q->m_Dato) {
        p->m_Sons.push_front(q); 
        p->m_Grado++; 
        q->m_Padre = p; 
        cout<<endl<<q->m_Dato<<" hijo de de "<<q->m_Padre->m_Dato<<endl;
        m_Roots.remove(q); 
    }
    else { 
        q->m_Sons.push_front(p); 
        q->m_Grado++; 
        p->m_Padre = q; 
        m_Roots.remove(p);
    }
}

int main()
{
    Binomial_Heap<int> aux;
    for(int i = 0; i < 20 ; i++)
    {
        aux.Insert(i);
    }
    cout<<endl<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Delete(18);
    aux.Delete(13);
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Delete(12);
    aux.Delete(17);
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Delete(17);
    aux.Delete(19);
    aux.Extrac_Min();
    aux.Extrac_Min();
    aux.Graficar();
    aux.showList();
    aux.showList();
    return 1;
}