/**
 *  Initialization Library,
 *  (C) Thorsten Ottosen 2002 -
 *  
 *   
 */

#include <algorithm>
#include <cassert>
#include <deque>
//#include <hash_set>
//#include <hash_map>
// HACK: Temporary hack by Allen to make this stuff work
//       This requires that vpr/vpr.h is included before using
#include VPR_HASH_MAP_INCLUDE
#include VPR_HASH_SET_INCLUDE
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
//#include <slist>
#include <stack>
#include <vector>

#include <boost/array.hpp>
#include <boost/call_traits.hpp>
#include <boost/static_assert.hpp>

namespace init
{
    /////////////////////////////////////////////////////////////////////////
    // Initialization interface
    /////////////////////////////////////////////////////////////////////////

    namespace Private
    {
        template< typename Container >
        class Comma_initializer;

        template< typename Container >
        class Map_data_initializer;
        
        template< typename Container >
        class Map_comma_initializer;

        template< typename T >
        class Any_container;
    }

    template< typename Container >
    Private::Comma_initializer<Container>      set_cont( Container& c );

    template< typename Container >
    Private::Map_comma_initializer<Container>  set_map( Container& c );

    /////////////////////////////////////////////////////////////////////////
    // Convenient enums interface
    /////////////////////////////////////////////////////////////////////////

    template< typename T >
    Private::Any_container<T>  enum_n( size_t n );

    template< typename T >
    Private::Any_container<T>  enum_n( size_t n, T step_size );

    template< typename T >
    Private::Any_container<T>  enum_n_from( size_t n, T from );

    template< typename T >
    Private::Any_container<T>  enum_n_from( size_t n, T from, T step_size );

    template< typename T >
    Private::Any_container<T>  renum_n_from( size_t n, T from );

    template< typename T >
    Private::Any_container<T>  renum_n_from( size_t n, T from, T step_size );

    template< typename T, typename Functor >
    Private::Any_container<T>  n_generated_by( T n, Functor generator );

    /////////////////////////////////////////////////////////////////////////
    // Fast enums interface
    /////////////////////////////////////////////////////////////////////////

    template< typename T, typename Container >
    Container  fenum_n( size_t n, const Container& );

    template< typename T, typename Container >
    Container  fenum_n( size_t n, T step_size, const Container& );

    template< typename T, typename Container >
    Container  fenum_n_from( size_t n, T from, const Container& );

    template< typename T, typename Container >
    Container  fenum_n_from( size_t n, T from, T step_size, const Container& );

    template< typename T, typename Container >
    Container  frenum_n_from( size_t n, T from, const Container& );

    template< typename T , typename Container >
    Container  frenum_n_from( size_t n, T from, T step_size, 
                              const Container& );

    template< typename T, typename Functor, typename Container >
    Container  n_generated_by( T n, Functor generator, const Container& );

    /////////////////////////////////////////////////////////////////////////
    // Implementation
    /////////////////////////////////////////////////////////////////////////

    namespace Private
    {
        /////////////////////////////////////////////////////////////////////
        // Insert policy
        /////////////////////////////////////////////////////////////////////

        using namespace std;
    #if defined(__GNUC__) && __GNUC__ == 3
        using namespace __gnu_cxx;
    #endif
    #if defined(__MWERKS__)
        using namespace Metrowerks;
    #endif
    
        /**
         * Define how elements should be inserted by the comma 
         * operator. To support a container, one must define
         * 
         *    insert( Container& to, Container::value_type value );
         *
         * The Default is to use 'Container::push_back()'
         * which will apply to 'std::vector<>, std::list<>, std::deque<>'.
         *    
         * The following classes are supported
         * ( a '*' means the container is non-standard, but supported
         * because it is frequantly used and probaly will be part of
         * the new standard library):
         * 
         *  vector 
         *  deque 
         *  list 
         *  slist *
         *  set 
         *  multiset 
         *  hash_set *
         *  hash_multiset * 
         *  
         *  map 
         *  multimap 
         *  hash_map *
         *  hash_multimap *
         *  
         *  stack 
         *  queue 
         *  priority_queue 
         */

        template< typename Container, typename T >
        struct Insert_policy
        {
            typedef typename boost::call_traits<T>::param_type   param_type;

            void insert( Container& to, param_type value )
            {
                to.push_back( value );
            }
            
            
            
            template< typename Container2 >
            void insert( Container& to, const Container2& from )
            {
                to.insert( from.begin(), from.end() );
            }
        };



/*
        template< typename T >          
        struct Insert_policy< slist<T>, T >
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( slist<T>& to, param_type value )
            {
                to.insert( to.end(), value );
            }
        };
*/


        template< typename T>
        struct Insert_policy< std::set<T>, T >
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( std::set<T>& s, param_type value )
            {
                s.insert( value );
            }
        };



        template< typename T >
        struct Insert_policy< std::multiset<T>, T >
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( std::multiset<T>& s, param_type value )
            {
                s.insert( value );
            }
        }; 

        
        
        template< typename T>
        struct Insert_policy< std::hash_set<T>, T >
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( std::hash_set<T>& s, param_type value )
            {
                s.insert( value );
            }
        };



        template< typename T >
        struct Insert_policy< std::hash_multiset<T>, T >
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( std::hash_multiset<T>& s, param_type value )
            {
                s.insert( value );
            }
        }; 


        
        template< typename T >          
        struct Insert_policy< std::stack<T>, T >
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( std::stack<T>& to, param_type value )
            {
                to.push( value );
            }
        };



        template< typename T >          
        struct Insert_policy< std::queue<T>, T > 
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( std::queue<T>& to, param_type value )
            {
                to.push( value );
            }
        };



        template< typename T >          
        struct Insert_policy< std::priority_queue<T>, T >
        {
            typedef typename boost::call_traits<T>::param_type param_type;

            void insert( std::priority_queue<T>& to, param_type value )
            {
                to.push( value );
            }
        };

        /////////////////////////////////////////////////////////////////////
        // Clear policy
        /////////////////////////////////////////////////////////////////////

        template< typename Container, typename T >
        struct Clear_policy
        {
            void clear( Container& c )
            {
                c.clear();
            }
        };



        template< typename T >
        struct Clear_policy< std::stack<T>, T >
        {
            void clear( std::stack<T>& c )
            {
                c = std::stack<T>();
            }
        };



        template< typename T >
        struct Clear_policy< std::queue<T>, T >
        {
            void clear( std::queue<T>& c )
            {
                c = std::queue<T>();
            }
        };


        
        template< typename T >
        struct Clear_policy< std::priority_queue<T>, T >
        {
            void clear( std::priority_queue<T>& c )
            {
                c = std::priority_queue<T>();
            }
        };

        /////////////////////////////////////////////////////////////////////
        // class 'Comma_initializer'
        /////////////////////////////////////////////////////////////////////

        /**
         *  Template class which forwards insertions to the
         *  container class.
         */ 
        template< typename Container >
        class Comma_initializer : 
        Insert_policy<Container, typename Container::value_type>,
        Clear_policy< Container, typename Container::value_type>
        {
            Comma_initializer();
            typedef typename Container::value_type       value_type;
            typedef typename 
            boost::call_traits<value_type>::param_type   param_type;

        public: 

            explicit Comma_initializer( Container& c ) : c_( c ) 
            {
            }



            Comma_initializer& operator,( param_type value )
            {
                insert( value );
                return *this;
            }



            /**
             *  Should only be used with first value. The operator
             *  gives a nice syntax for initializing the container.
             */
            Comma_initializer& operator=( param_type first_value )
            {
                Clear_policy<Container, value_type>::clear( c_ );
                insert( first_value );
                return *this;
            }



            /**
             *  Should only be used with first value. The operator
             *  gives a nice syntax for appending to the container.
             */
            Comma_initializer& operator+=( param_type first_value )
            {
                insert( first_value );
                return *this;
            }

        private:

            void insert( param_type value )
            {
                Insert_policy<Container,value_type>::insert( c_, value );
            }


            Container& c_;

        }; // class 'Comma_initializer'

        /////////////////////////////////////////////////////////////////////
        // Map insert policy
        /////////////////////////////////////////////////////////////////////

        template< typename Container >          
        struct Map_insert_policy
        {
            typedef typename boost::call_traits< typename Container::
            value_type>::param_type  param_type;

            void insert( Container& to, param_type value )
            {
                to.insert( value );
            }
        };

        /////////////////////////////////////////////////////////////////////
        // class 'Map_comma_initializer
        /////////////////////////////////////////////////////////////////////

        template< typename Container >
        class Map_data_initializer
        {
            typedef typename Container::data_type   data_type;
            typedef typename boost::call_traits<data_type>::param_type
                                                    data_param_type;
            typedef Map_comma_initializer<Container>
                                                    Key_initializer;
        public:
            Map_data_initializer( Key_initializer& ki ) 
              : key_init_( ki ) 
            { }
            
            
            
            Key_initializer& operator,( data_param_type data )
            {
                key_init_.insert( data );
                return key_init_;
            }
            
        private:
            
            Key_initializer&  key_init_;
        
        }; // class 'Map_data_initializer'
        
        
        
        
        template< typename Container >
        class Map_comma_initializer : Map_insert_policy< Container >  
        {
            Map_comma_initializer();

            typedef typename Container::value_type  value_type;
            typedef typename Container::key_type    key_type;
            typedef typename Container::data_type   data_type;
            
            typedef typename boost::call_traits<value_type>::param_type
            param_type;

            typedef typename boost::call_traits<key_type>::param_type
            key_param_type;
            
            typedef typename boost::call_traits<data_type>::param_type
                                                    data_param_type;
            
            typedef Map_data_initializer<Container>
                                                  Data_initializer;

        public:

            Map_comma_initializer( Container& c ) : c_( c ), 
                data_init_( *this  )
             {
            }



            Data_initializer& operator=( key_param_type key )
            {
                c_.clear();
                pair_.first = key;
                return data_init_;
            }



            Data_initializer& operator+=( key_param_type key )
            {
                pair_.first = key;
                return data_init_;
            }



            Data_initializer& operator,( key_param_type key )
            {
                pair_.first = key;
                return data_init_;
            }


            void insert( data_param_type data )
            {
                pair_.second = data;
                insert( pair_ );
            }
        
        private:

            void insert( param_type value )
            {
                Map_insert_policy<Container>::
                insert( c_, value );
            }



            Container&                     c_;
            Data_initializer               data_init_;
            std::pair<key_type,data_type>  pair_;

        }; // class 'Map_comma_initializer'


    } // namespace 'Private'

    /////////////////////////////////////////////////////////////////////////
    // Initialization 
    /////////////////////////////////////////////////////////////////////////

    /**
     *  Template function that gives the initializer
     *  a clean and safe interface.
     */ 
    template< typename Container >
    Private::Comma_initializer<Container>      set_cont( Container& c )
    {
        return Private::Comma_initializer<Container>( c );
    }



    template< typename Container >
    Private::Map_comma_initializer<Container>  set_map( Container& c )
    {
        return Private::Map_comma_initializer<Container>( c );
    }

    /////////////////////////////////////////////////////////////////////////
    // Convenient enumeration
    /////////////////////////////////////////////////////////////////////////

    namespace Private
    {
        using namespace std;

        template< typename T > 
        struct Step_generator
        {
            Step_generator( T initial_value, T step_size ) 
            : value_( initial_value ), step_size_( step_size )
            {
            }



            T operator()()
            {
                T res = value_;
                value_ += step_size_;
                return res;
            }

        private:
            T        value_;
            const T  step_size_;

        }; // struct 'Step_generator'


        
        template< typename Container >
        struct Push_copy : 
            Insert_policy<Container, typename Container::value_type >
        {
            typedef typename Container::value_type value_type;
            typedef typename boost::call_traits<value_type>::param_type
                param_type;
            
            Push_copy( Container& c ) : c_( c )
            {
            }
            
            
            
            void operator()( param_type value )
            {
                Insert_policy<Container, typename Container::value_type>::
                    insert( c_, value );
            }
            
        private:
            Container& c_;
        
        }; // struct 'Push_copy'
        
        

        /**
         * Class that converts implicitly to all
         * supported containers. If one supplies a 
         * second template parameter (which describes the
         * container type that should be converted to)
         * the converiosn will be without overhead.
         */
        template< typename T >
        class Any_container
        {
            std::vector<T>  holder_;

        public: // construction

            Any_container( const vector<T>& v ) : holder_( v )
            {
            }

        public: // conversion    

            operator vector<T>() 
            {
                return holder_;
            }



            operator deque<T>() 
            {
                return deque<T>( holder_.begin(), holder_.end() );
            }



            operator list<T>() 
            {
                return list<T>( holder_.begin(), holder_.end() );
            }



/*
            operator slist<T>() 
            {
                return slist<T>( holder_.begin(), holder_.end() );
            }
*/



            operator set<T>() 
            {
                return set<T>( holder_.begin(), holder_.end() );
            }



            operator multiset<T>() 
            {
                return multiset<T>( holder_.begin(), holder_.end() );
            }


            
            operator hash_set<T>()
            {
                return hash_set<T>( holder_.begin(), holder_.end() );
            }
            
            
            
            operator hash_multiset<T>()
            {
                return hash_multiset<T>( holder_.begin(), holder_.end() ); 
            }
            
                
                
            template< size_t size >
            operator boost::array<T,size>()
            {
                assert( size == holder_.size() );
                boost::array<T,size> a;
                copy( holder_.begin(), holder_.end(), a.begin() );
                return a;
            }
            
            
            
            operator stack<T>() 
            {
                typedef stack<T> type;
                type s;
                for_each( holder_.begin(), holder_.end(), 
                          Push_copy<type>( s ) );
                return s;
            }
            
            
            
            operator queue<T>() 
            {
                typedef queue<T> type;
                type q;
                for_each( holder_.begin(), holder_.end(), 
                          Push_copy<type>( q ) );
                return q;
            }
            
            
            
            operator priority_queue<T>() 
            {
                typedef priority_queue<T> type;
                type pq;
                for_each( holder_.begin(), holder_.end(), 
                          Push_copy<type>( pq ) );
                return pq;
            }

        }; // class 'Any_container'

    } // namespace 'Private'


    
    /** 
     * '{ 1, ..., n }'
     */
    template< typename T >
    Private::Any_container<T> 
    enum_n( size_t n )
    {
        return Private::Any_container<T>( fenum_n<T>( n, std::vector<T>() ) );
    }


    /**
     * '{ 1 + 0*step_size, ..., 1 + (n-1)*step_size }' 
     */
    template< typename T >
    Private::Any_container<T>  
    enum_n( size_t n, T step_size )
    {
        return Private::Any_container<T>( fenum_n( n, step_size, 
                                                   std::vector<T>() ) );
    }



    /**
     *  '{ from + 0, ..., from + (n-1) }'
     */
    template< typename T >
    Private::Any_container<T>
    enum_n_from( size_t n, T from )
    {
        return Private::Any_container<T>( fenum_n_from( n, from, 
                                                        std::vector<T>() ) );
    }


    
   /**
     * '{ from + 0*step_size, ..., from + (n-1)*step_size }'
     */ 
    template< typename T >
    Private::Any_container<T> 
    enum_n_from( size_t n, T from, T step_size )
    {
        return Private::Any_container<T>( fenum_n_from( n, from, step_size, 
                                                        std::vector<T>() ) );
    }


    
    /** 
     * '{ from, from - 1, ..., from - (n-1) }
     */
    template< typename T >
    Private::Any_container<T> 
    renum_n_from( size_t n, T from )
    {
        return Private::Any_container<T>( frenum_n_from( n, from, 
                                                         std::vector<T>() ) );
    }


    
    /**
     * '{ from - 0*step_size, ..., from - (n-1)*step_size }'
     */
    template< typename T >
    Private::Any_container<T>
    renum_n_from( size_t n, T from, T step_size )
    {
        return Private::Any_container<T>( frenum_n_from( n, from, step_size, 
                                                         std::vector<T>() ) );
    }

    

    /** 
     * Call the 'generator' 'n' times and return the results
     * in a container.
     */
    template< typename T, typename Functor >
    Private::Any_container<T>  
    n_generated_by( T n, Functor generator )
    {
        return Private::Any_container<T>( n_generated_by( n, generator, 
                                                          std::vector<T>() ) );      
    }

    /////////////////////////////////////////////////////////////////////////
    // Fast enumeration
    /////////////////////////////////////////////////////////////////////////

    template< typename T, typename Container >
    Container 
    fenum_n( size_t n, const Container& c )
    {
        return fenum_n( n, T( 1 ), c );
    }



    template< typename T, typename Container >
    Container  
    fenum_n( size_t n, T step_size, const Container& c )
    {
        return fenum_n_from( n, T( 1 ), step_size, c );
    }


  
    template< typename T, typename Container >
    Container  
    fenum_n_from( size_t n, T from, const Container& c )
    {
        return fenum_n_from( n, from, T( 1 ), c );
    }


 
    template< typename T, typename Container >
    Container  
    fenum_n_from( size_t n, T from, T step_size, const Container& )
    {
        using namespace Private;
        Container c( n );
        std::generate( c.begin(), c.end(), 
                       Step_generator<T>( from, step_size ) );
        return c;
    }


    
    template< typename T, typename Container >
    Container  
    frenum_n_from( size_t n, T from, const Container& c )
    {
        return fenum_n_from( n, from, T( -1 ), c );
    }


    
    template< typename T , typename Container >
    Container  
    frenum_n_from( size_t n, T from, T step_size, 
                   const Container& c )
    {
        return fenum_n_from( n, from, -step_size, c );
    }



    template< typename T, typename Functor, typename Container >
    Container 
    n_generated_by( T n, Functor generator, const Container& ) 
    {
        using namespace Private;
        Container c( static_cast<size_t>( n ) );  
        std::generate( c.begin(), c.end(), 
                       generator );
        return c;
    }

} // namespace 'init'
