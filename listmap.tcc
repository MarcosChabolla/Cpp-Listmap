// $Id: listmap.tcc,v 1.9 2016/07/20 20:07:26 akhatri Exp $

#include "listmap.h"
#include "trace.h"
#include "xpair.h"
#include "util.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
   
   for(auto it = begin(); it != end();) {
     auto tbd = it;
     ++it;
     erase(tbd);
   }
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);
   
   node * nn = new node(anchor()->next,anchor(),pair);
   
   //Need to check if key already exists
   //In that case, replace value with new
   //value. Implement find() first
   
   auto it = begin();
   
   //If this is the first node inserted into the list
   if(it==end())
   {
       
      anchor()->next->next = nn;
      nn->prev = anchor()->next;
      nn->next = anchor();
      anchor()->prev = nn;
   }
   else
   {
       
       if(find(pair.first)==end())
       {
           //cout<<"Did not find "<<pair.first<< " inserting"<<endl;
           
           while(it!=end())
           {
             
               if(pair.first < it->first)
               {
                   //If value type to be inserted
                   //Belongs in the front of the list
                   if(it==begin())
                   {
                       
                       nn->next = anchor()->next;
                       nn->next->prev = nn;
                       anchor()->next = nn;
                       nn->prev = anchor();
                       return iterator(nn);
                   }
                   //If the value type to be inserted belongs
                   //anywhere else
                   else
                   {
                       //cout<<pair.first<<" is less than "<<it->first<<endl;
                       auto currNode = it.where;
                       currNode->prev->next = nn;
                       nn->prev = currNode->prev;
                       nn->next = currNode;
                       currNode->prev = nn;
                       return iterator(nn);
                   }
                   
               }
               ++it;
           }
           //If the value type to be inserted
           //Belongs at the back of the list
           if(anchor()->prev)
           {
               anchor()->prev->next = nn;
           }
           nn->prev = anchor()->prev;
           anchor()->prev = nn;
           nn->next = anchor();
       }
       //If key already exists and value
       //needs to be replaced
       else if(find(pair.first)!=end())
       {
           auto it2 = find(pair.first);
           it2->second = pair.second;
       }
   }
   
   return iterator(nn);
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   TRACE ('l', that);
  
   auto it = anchor();
   //Can't use begin()/end() here for some reason
   //Anyways, iterate through list and look for 
   //"that" and return it. If not found, return nullptr?
   for(it=anchor()->next; it!=anchor();it=it->next){
       if(it->value.first == that){
           return it;
       }
   }
  
   return end();
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) 
{
    
   
   
    auto tempPos = position.where;
    auto ret = tempPos->next;
    tempPos->next->prev = tempPos->prev;
    tempPos->prev->next = tempPos->next;

    position.erase();

   return iterator(ret);
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::iterator::erase() 
{
   TRACE ('l', where);
   delete where;
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

