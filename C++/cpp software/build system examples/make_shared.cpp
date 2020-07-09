#include <boost/shared_ptr.hpp>
#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <cstddef>
#include <new>

template< std::size_t N, std::size_t A > struct sp_aligned_storage
{
    union type
    {
        char data_[ N ];
        typename boost::type_with_alignment< A >::type align_;
    };
};

template< class T > class sp_ms_deleter
{
private:

    typedef typename sp_aligned_storage< sizeof( T ), boost::alignment_of< T >::value >::type storage_type;

    bool initialized_;
    storage_type storage_;

private:

    void destroy()
    {
        if( initialized_ )
        {
            reinterpret_cast< T* >( storage_.data_ )->~T();
            initialized_ = false;
        }
    }

public:

    sp_ms_deleter(): initialized_( false )
    {
    }

    ~sp_ms_deleter()
    {
        destroy();
    }

    void operator()( T * )
    {
        destroy();
    }

    void * address()
    {
        return storage_.data_;
    }

    void set_initialized()
    {
        initialized_ = true;
    }
};

// Zero-argument versions
//
// Used even when variadic templates are available because of the new T() vs new T issue

template< class T > boost::shared_ptr< T > make_shared()
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >() );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T();
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A > boost::shared_ptr< T > allocate_shared( A const & a )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >(), a );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T();
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

#if defined( __GXX_EXPERIMENTAL_CXX0X__ ) && !defined( __STRICT_ANSI__ )

// Variadic templates, rvalue reference

template< class T> T forward( T t )
{
    return t;
}

template< class T, class... Args > boost::shared_ptr< T > make_shared( Args && ... args )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >() );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( forward<Args>( args )... );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A, class... Args > boost::shared_ptr< T > allocate_shared( A const & a, Args && ... args )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >(), a );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( forward<Args>( args )... );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

#else

// C++03 version

template< class T, class A1 > boost::shared_ptr< T > make_shared( A1 const & a1 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >() );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A, class A1 > boost::shared_ptr< T > allocate_shared( A const & a, A1 const & a1 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >(), a );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A1, class A2 > boost::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >() );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1, a2 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A, class A1, class A2 > boost::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >(), a );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1, a2 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A1, class A2, class A3 > boost::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >() );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1, a2, a3 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A, class A1, class A2, class A3 > boost::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >(), a );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1, a2, a3 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A1, class A2, class A3, class A4 > boost::shared_ptr< T > make_shared( A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >() );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1, a2, a3, a4 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

template< class T, class A, class A1, class A2, class A3, class A4 > boost::shared_ptr< T > allocate_shared( A const & a, A1 const & a1, A2 const & a2, A3 const & a3, A4 const & a4 )
{
    boost::shared_ptr< T > pt( static_cast< T* >( 0 ), sp_ms_deleter< T >(), a );

    sp_ms_deleter< T > * pd = boost::get_deleter< sp_ms_deleter< T > >( pt );

    void * pv = pd->address();

    new( pv ) T( a1, a2, a3, a4 );
    pd->set_initialized();

    return boost::shared_ptr< T >( pt, static_cast< T* >( pv ) );
}

#endif

class X
{
private:

    X( X const & );
    X & operator=( X const & );

public:

    static int instances;

    int v;

    explicit X( int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0 ): v( a1+a2+a3+a4 )
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }
};

int X::instances = 0;

#include <boost/detail/lightweight_test.hpp>
#include <boost/weak_ptr.hpp>

int main()
{
    {
        boost::shared_ptr< int > pi = make_shared< int >();

        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( *pi == 0 );
    }

    {
        boost::shared_ptr< int > pi = allocate_shared< int >( std::allocator<int>() );

        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( *pi == 0 );
    }

    {
        boost::shared_ptr< int > pi = make_shared< int >( 5 );

        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( *pi == 5 );
    }

    {
        boost::shared_ptr< int > pi = allocate_shared< int >( std::allocator<int>(), 5 );

        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( *pi == 5 );
    }

    BOOST_TEST( X::instances == 0 );

    {
        boost::shared_ptr< X > pi = make_shared< X >();
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 0 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }


    {
        boost::shared_ptr< X > pi = allocate_shared< X >( std::allocator<void>() );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 0 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = make_shared< X >( 1 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = allocate_shared< X >( std::allocator<void>(), 1 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = make_shared< X >( 1, 2 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1+2 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = allocate_shared< X >( std::allocator<void>(), 1, 2 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1+2 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = make_shared< X >( 1, 2, 3 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1+2+3 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = allocate_shared< X >( std::allocator<void>(), 1, 2, 3 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1+2+3 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = make_shared< X >( 1, 2, 3, 4 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1+2+3+4 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    {
        boost::shared_ptr< X > pi = allocate_shared< X >( std::allocator<void>(), 1, 2, 3, 4 );
        boost::weak_ptr<X> wp( pi );

        BOOST_TEST( X::instances == 1 );
        BOOST_TEST( pi.get() != 0 );
        BOOST_TEST( pi->v == 1+2+3+4 );

        pi.reset();

        BOOST_TEST( X::instances == 0 );
    }

    return boost::report_errors();
}
