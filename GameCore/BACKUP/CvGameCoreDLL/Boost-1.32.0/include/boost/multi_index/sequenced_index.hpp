/* Copyright 2003-2004 Joaqu�n M L�pez Mu�oz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_SEQUENCED_INDEX_HPP
#define BOOST_MULTI_INDEX_SEQUENCED_INDEX_HPP

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/call_traits.hpp>
#include <boost/detail/no_exceptions_support.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/multi_index/detail/access_specifier.hpp>
#include <boost/multi_index/detail/index_iterator.hpp>
#include <boost/multi_index/detail/seq_index_node.hpp>
#include <boost/multi_index/detail/seq_index_ops.hpp>
#include <boost/multi_index/detail/safe_mode.hpp>
#include <boost/multi_index/detail/scope_guard.hpp>
#include <boost/multi_index/sequenced_index_fwd.hpp>
#include <boost/tuple/tuple.hpp>
#include <cstddef>
#include <functional>
#include <utility>

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
#define BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT                          \
  detail::scope_guard BOOST_JOIN(check_invariant_,__LINE__)=                 \
    detail::make_obj_guard(*this,&sequenced_index::check_invariant_);        \
  BOOST_JOIN(check_invariant_,__LINE__).touch();
#else
#define BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT
#endif

namespace boost{

namespace multi_index{

namespace detail{

/* sequenced_index adds a layer of sequenced indexing to a given Super */

template<typename Super,typename TagList>

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
class sequenced_index:
  BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super,
  public index_proxy<sequenced_index_node<typename Super::node_type> >
#else
class sequenced_index:
  BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super,
  public safe_container<sequenced_index<Super,TagList> >
#endif
#else
class sequenced_index:
  BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super
#endif

{ 
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)&&\
    BOOST_WORKAROUND(__MWERKS__,<=0x3003)
/* The "ISO C++ Template Parser" option in CW8.3 has a problem with the
 * lifetime of const references bound to temporaries --precisely what
 * scopeguards are.
 */

#pragma parse_mfunc_templ off
#endif

protected:
  typedef sequenced_index_node<typename Super::node_type> node_type;

public:
  /* types */

  typedef typename node_type::value_type             value_type;
  typedef tuples::null_type                          ctor_args;
  typedef typename Super::final_allocator_type       allocator_type;
  typedef typename allocator_type::reference         reference;
  typedef typename allocator_type::const_reference   const_reference;

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
  typedef index_iterator<node_type>                  iterator;
  typedef index_iterator<node_type>                  const_iterator;
#else
  typedef index_iterator<node_type,sequenced_index>  iterator;
  typedef index_iterator<node_type,sequenced_index>  const_iterator;
#endif
#else
  typedef index_iterator<node_type>                  iterator;
  typedef index_iterator<node_type>                  const_iterator;
#endif

  typedef std::size_t                                size_type;      
  typedef std::ptrdiff_t                             difference_type;
  typedef typename allocator_type::pointer           pointer;
  typedef typename allocator_type::const_pointer     const_pointer;
  typedef typename
    boost::reverse_iterator<iterator>                reverse_iterator;
  typedef typename
    boost::reverse_iterator<const_iterator>          const_reverse_iterator;
  typedef typename TagList::type                     tag_list;

protected:
  typedef typename Super::final_node_type     final_node_type;
  typedef tuples::cons<
    ctor_args, 
    typename Super::ctor_args_list>           ctor_args_list;
  typedef typename mpl::push_front<
    typename Super::index_type_list,
    sequenced_index>::type                    index_type_list;
  typedef typename mpl::push_front<
    typename Super::iterator_type_list,
    iterator>::type                           iterator_type_list;
  typedef typename mpl::push_front<
    typename Super::const_iterator_type_list,
    const_iterator>::type                     const_iterator_type_list;
  typedef typename Super::copy_map_type       copy_map_type;

private:
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
  typedef index_proxy<sequenced_index_node<
      typename Super::node_type> >           safe_super;
#else
  typedef safe_container<sequenced_index>    safe_super;
#endif
#endif

  typedef typename call_traits<value_type>::param_type value_param_type;

public:

  /* construct/copy/destroy
   * Default and copy ctors are in the protected section as indices are
   * not supposed to be created on their own. No range ctor either.
   */

  sequenced_index<Super,TagList>& operator=(
    const sequenced_index<Super,TagList>& x)
  {
    this->final()=x.final();
    return *this;
  }

  template <class InputIterator>
  void assign(InputIterator first,InputIterator last)
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    clear();
    for(;first!=last;++first)push_back(*first);
  }

  void assign(size_type n,value_param_type value)
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    clear();
    for(size_type i=0;i<n;++i)push_back(value);
  }
    
  allocator_type get_allocator()const
  {
    return this->final().get_allocator();
  }

  /* iterators */

  iterator               begin()
    {return make_iterator(node_type::from_impl(header()->next()));}
  const_iterator         begin()const
    {return make_iterator(node_type::from_impl(header()->next()));}
  iterator               end(){return make_iterator(header());}
  const_iterator         end()const{return make_iterator(header());}
  reverse_iterator       rbegin(){return make_reverse_iterator(end());}
  const_reverse_iterator rbegin()const{return make_reverse_iterator(end());}
  reverse_iterator       rend(){return make_reverse_iterator(begin());}
  const_reverse_iterator rend()const{return make_reverse_iterator(begin());}

  /* capacity */

  bool      empty()const{return this->final_empty_();}
  size_type size()const{return this->final_size_();}
  size_type max_size()const{return this->final_max_size_();}

  void resize(size_type n,value_param_type x=value_type())
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(n>size())insert(end(),n-size(),x);
    else if(n<size()){
      iterator it=begin();
      std::advance(it,n);
      erase(it,end());
    }   
  }

  /* access: no non-const versions provided as sequenced_index
   * handles const elements.
   */

  const_reference front()const{return *begin();}
  const_reference back()const{return *--end();}

  /* modifiers */

  std::pair<iterator,bool> push_front(value_param_type x)
                             {return insert(begin(),x);}
  void                     pop_front(){erase(begin());}
  std::pair<iterator,bool> push_back(value_param_type x)
                             {return insert(end(),x);}
  void                     pop_back(){erase(--end());}

  std::pair<iterator,bool> insert(iterator position,value_param_type x)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    std::pair<final_node_type*,bool> p=this->final_insert_(x);
    if(p.second)relink(position.get_node(),p.first);
    return std::pair<iterator,bool>(make_iterator(p.first),p.second);
  }

  void insert(iterator position,size_type n,value_param_type x)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    for(size_type i=0;i<n;++i)insert(position,x);
  }
 
  template<typename InputIterator>
  void insert(iterator position,InputIterator first,InputIterator last)
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    for(;first!=last;++first)insert(position,*first);
  }

  iterator erase(iterator position)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    this->final_erase_(static_cast<final_node_type*>(position++.get_node()));
    return position;
  }
  
  iterator erase(iterator first,iterator last)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(first,*this);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(last,*this);
    BOOST_MULTI_INDEX_CHECK_VALID_RANGE(first,last);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    while(first!=last){
      first=erase(first);
    }
    return first;
  }

  bool replace(iterator position,value_param_type x)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    return this->final_replace_(
      x,static_cast<final_node_type*>(position.get_node()));
  }

  template<typename Modifier>
  bool modify(iterator position,Modifier mod)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer on safe mode code chokes if this
     * this is not added. Left it for all compilers as it does no
     * harm.
     */

    position.detach();
#endif

    return this->final_modify_(
      mod,static_cast<final_node_type*>(position.get_node()));
  }

  void swap(sequenced_index<Super,TagList>& x)
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    this->final_swap_(x.final());
  }

  void clear()
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    erase(begin(),end());
  }

  /* list operations */

  void splice(iterator position,sequenced_index<Super,TagList>& x)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_CHECK_DIFFERENT_CONTAINER(*this,x);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    iterator first=x.begin(),last=x.end();
    while(first!=last){
      if(insert(position,*first).second)x.erase(first++);
      else ++first;
    }
  }

  void splice(iterator position,sequenced_index<Super,TagList>& x,iterator i)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(i);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(i);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(i,x);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(x==*this){
      if(position!=i)relink(position.get_node(),i.get_node());
    }
    else{
      if(insert(position,*i).second){

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer has a hard time with safe mode, and the following
     * workaround is needed. Left it for all compilers as it does no
     * harm.
     */
        i.detach();
        x.erase(x.make_iterator(i.get_node()));
#else
        x.erase(i);
#endif

      }
    }
  }

  void splice(
    iterator position,sequenced_index<Super,TagList>& x,
    iterator first,iterator last)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(first,x);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(last,x);
    BOOST_MULTI_INDEX_CHECK_VALID_RANGE(first,last);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(x==*this){
      BOOST_MULTI_INDEX_CHECK_OUTSIDE_RANGE(position,first,last);
      if(position!=last)relink(
        position.get_node(),first.get_node(),last.get_node());
    }
    else{
      while(first!=last){
        if(insert(position,*first).second)x.erase(first++);
        else ++first;
      }
    }
  }

  void remove(value_param_type value)
  {
    sequenced_index_remove(
      *this,std::bind2nd(std::equal_to<value_type>(),value));
  }

  template<typename Predicate>
  void remove_if(Predicate pred)
  {
    sequenced_index_remove(*this,pred);
  }

  void unique()
  {
    sequenced_index_unique(*this,std::equal_to<value_type>());
  }

  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred)
  {
    sequenced_index_unique(*this,binary_pred);
  }

  void merge(sequenced_index<Super,TagList>& x)
  {
    sequenced_index_merge(*this,x,std::less<value_type>());
  }

  template <typename Compare>
  void merge(sequenced_index<Super,TagList>& x,Compare comp)
  {
    sequenced_index_merge(*this,x,comp);
  }

  void sort()
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_sort(header(),std::less<value_type>());
  }

  template <typename Compare>
  void sort(Compare comp)
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_sort(header(),comp);
  }

  void reverse()
  {
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_node_impl::reverse(header()->impl());
  }

  /* relocate operations */

  void relocate(iterator position,iterator i)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(i);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(i);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(i,*this);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(position!=i)relink(position.get_node(),i.get_node());
  }

  void relocate(iterator position,iterator first,iterator last)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(first,*this);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(last,*this);
    BOOST_MULTI_INDEX_CHECK_VALID_RANGE(first,last);
    BOOST_MULTI_INDEX_CHECK_OUTSIDE_RANGE(position,first,last);
    BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(position!=last)relink(
      position.get_node(),first.get_node(),last.get_node());
  }
    
BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:
  sequenced_index(const ctor_args_list& args_list,const allocator_type& al):
    Super(args_list.get_tail(),al)

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)&&\
    BOOST_WORKAROUND(BOOST_MSVC,<1300)
    ,safe_super(final_header())
#endif

  {
    header()->prior()=header()->next()=header()->impl();
  }

  sequenced_index(const sequenced_index<Super,TagList>& x):
    Super(x)

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)&&\
    BOOST_WORKAROUND(BOOST_MSVC,<1300)
    ,safe_super(final_header())
#endif

  {
    /* The actual copying takes place in subsequent call to copy_().
     */
  }

  ~sequenced_index()
  {
    /* the container is guaranteed to be empty by now */
  }

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  iterator       make_iterator(node_type* node){return iterator(node,this);}
  const_iterator make_iterator(node_type* node)const
    {return const_iterator(node,const_cast<sequenced_index*>(this));}
#else
  iterator       make_iterator(node_type* node){return iterator(node);}
  const_iterator make_iterator(node_type* node)const
                   {return const_iterator(node);}
#endif

  void copy_(const sequenced_index<Super,TagList>& x,const copy_map_type& map)
  {
    node_type* org=x.header();
    node_type* cpy=header();
    do{
      node_type* next_org=node_type::from_impl(org->next());
      node_type* next_cpy=map.find(static_cast<final_node_type*>(next_org));
      cpy->next()=next_cpy->impl();
      next_cpy->prior()=cpy->impl();
      org=next_org;
      cpy=next_cpy;
    }while(org!=x.header());

    Super::copy_(x,map);
  }

  node_type* insert_(value_param_type v,node_type* x)
  {
    node_type* res=static_cast<node_type*>(Super::insert_(v,x));
    if(res==x)link(x);
    return res;
  }

  node_type* insert_(value_param_type v,node_type* position,node_type* x)
  {
    node_type* res=static_cast<node_type*>(Super::insert_(v,position,x));
    if(res==x)link(x);
    return res;
  }

  void erase_(node_type* x)
  {
    unlink(x);
    Super::erase_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    detach_iterators(x);
#endif
  }

  void swap_(sequenced_index<Super,TagList>& x)
  {
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    safe_super::swap(x);
#endif

    Super::swap_(x);
  }

  bool replace_(value_param_type v,node_type* x)
  {
    return Super::replace_(v,x);
  }

  bool modify_(node_type* x)
  {
    BOOST_TRY{
      if(!Super::modify_(x)){
        unlink(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
        detach_iterators(x);
#endif

        return false;
      }
      else return true;
    }
    BOOST_CATCH(...){
      unlink(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
      detach_iterators(x);
#endif

      BOOST_RETHROW;
    }
    BOOST_CATCH_END
  }

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const
  {
    if(size()==0||begin()==end()){
      if(size()!=0||begin()!=end()||
         header()->next()!=header()->impl()||
         header()->prior()!=header()->impl())return false;
    }
    else{
      size_type s=0;
      for(const_iterator it=begin(),it_end=end();it!=it_end;++it,++s){
        if(it.get_node()->next()->prior()!=it.get_node()->impl())return false;
        if(it.get_node()->prior()->next()!=it.get_node()->impl())return false;
      }
      if(s!=size())return false;
    }

    return Super::invariant_();
  }

  /* This forwarding function eases things for the boost::mem_fn construct
   * in BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT. Actually,
   * final_check_invariant is already an inherited member function of index.
   */
  void check_invariant_()const{this->final_check_invariant_();}
#endif

private:
  node_type* header()const{return this->final_header();}

  void link(node_type* x)
  {
    sequenced_index_node_impl::link(x->impl(),header()->impl());
  };

  static void unlink(node_type* x)
  {
    sequenced_index_node_impl::unlink(x->impl());
  }

  static void relink(node_type* position,node_type* x)
  {
    sequenced_index_node_impl::relink(position->impl(),x->impl());
  }

  static void relink(node_type* position,node_type* first,node_type* last)
  {
    sequenced_index_node_impl::relink(
      position->impl(),first->impl(),last->impl());
  }

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  void detach_iterators(node_type* x)
  {
    iterator it=make_iterator(x);
    safe_mode::detach_equivalent_iterators(it);
  }
#endif

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)&&\
    BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif
};

/* comparison */

template<
  typename Super1,typename TagList1,
  typename Super2,typename TagList2
>
bool operator==(
  const sequenced_index<Super1,TagList1>& x,
  const sequenced_index<Super2,TagList2>& y)
{
  return x.size()==y.size()&&std::equal(x.begin(),x.end(),y.begin());
}

template<
  typename Super1,typename TagList1,
  typename Super2,typename TagList2
>
bool operator<(
  const sequenced_index<Super1,TagList1>& x,
  const sequenced_index<Super2,TagList2>& y)
{
  return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}

template<
  typename Super1,typename TagList1,
  typename Super2,typename TagList2
>
bool operator!=(
  const sequenced_index<Super1,TagList1>& x,
  const sequenced_index<Super2,TagList2>& y)
{
  return !(x==y);
}

template<
  typename Super1,typename TagList1,
  typename Super2,typename TagList2
>
bool operator>(
  const sequenced_index<Super1,TagList1>& x,
  const sequenced_index<Super2,TagList2>& y)
{
  return y<x;
}

template<
  typename Super1,typename TagList1,
  typename Super2,typename TagList2
>
bool operator>=(
  const sequenced_index<Super1,TagList1>& x,
  const sequenced_index<Super2,TagList2>& y)
{
  return !(x<y);
}

template<
  typename Super1,typename TagList1,
  typename Super2,typename TagList2
>
bool operator<=(
  const sequenced_index<Super1,TagList1>& x,
  const sequenced_index<Super2,TagList2>& y)
{
  return !(x>y);
}

/*  specialized algorithms */

template<typename Super,typename TagList>
void swap(
  sequenced_index<Super,TagList>& x,
  sequenced_index<Super,TagList>& y)
{
  x.swap(y);
}

} /* namespace multi_index::detail */

/* sequenced index specifier */

template <typename TagList>
struct sequenced
{
  BOOST_STATIC_ASSERT(detail::is_tag<TagList>::value);

  template<typename Super>
  struct node_class
  {
    typedef detail::sequenced_index_node<Super> type;
  };

  template<typename Super>
  struct index_class
  {
    typedef detail::sequenced_index<Super,TagList> type;
  };
};

} /* namespace multi_index */

} /* namespace boost */

#undef BOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT

#endif
