// C++11 (c) 2014 Vladimír Štill

#include "serialization.h"

using namespace serialization;

struct TestSerializationInternal {
    Test deserializeInt() {
        int testdata{ 42 };
        const char *ptr = reinterpret_cast< const char * >( &testdata );
        int got = Serializable< int >::deserialize( &ptr );
        assert_eq( got, 42, "Incorrect deserialization" );
    }

    Test fundamental() {
        assert_eq( Serializable< int >::size( 0 ), long( sizeof( int ) ), "size" );
        assert_eq( Serializable< double >::size( 0 ), long( sizeof( double ) ), "size" );
        assert_eq( Serializable< bool >::size( 0 ), long( sizeof( bool ) ), "size" );

        std::unique_ptr< char[] > buff{ new char[ sizeof( double ) ] };
        char *ptr = buff.get();

        Serializable< int >::serialize( 42, &ptr );
        assert_eq( buff.get() + sizeof( int ), ptr, "" );
        Serializable< bool >::serialize( true, &ptr );
        assert_eq( buff.get() + sizeof( int ) + sizeof( bool ), ptr, "" );

        ptr = buff.get();
        int x = Serializable< int >::deserialize( &ptr );
        assert_eq( x, 42, "" );
        assert_eq( buff.get() + sizeof( int ), ptr, "" );
        bool y = Serializable< bool >::deserialize( &ptr );
        assert( y, "" );
        assert_eq( buff.get() + sizeof( int ) + sizeof( bool ), ptr, "" );

        ptr = buff.get();
        Serializable< double >::serialize( 3.14, &ptr );
        assert_eq( buff.get() + sizeof( double ), ptr, "" );
        ptr = buff.get();
        double pi = Serializable< double >::deserialize( &ptr );
        assert_eq( pi, 3.14, "" );
        assert_eq( buff.get() + sizeof( double ), ptr, "" );
    }

    Test tuple() {
        using Tup = std::tuple< int, bool, long, int >;
        const int size = sizeof( int ) * 2 + sizeof( long ) + sizeof( bool );
        Tup t{ -1, true, 1991, 42 };

        std::unique_ptr< char[] > buff{ new char[ sizeof( Tup ) ] };
        char *ptr = buff.get();

        Serializable< Tup >::serialize( t, &ptr );
        assert_eq( buff.get() + size, ptr, "" );
        ptr = buff.get();
        Tup t2 = Serializable< Tup >::deserialize( &ptr );
        assert_eq( buff.get() + size, ptr, "" );
        assert_eq( t, t2, "" );
    }

    Test tuple2() {
        using Tup = std::tuple< int, long, bool, int, bool >;
        const int size = sizeof( int ) * 2 + sizeof( long ) + 2 * sizeof( bool );
        Tup t{ 0xff00ff00, 0x7700ff770077ff00, true, 42, false };

        std::unique_ptr< char[] > buff{ new char[ sizeof( Tup ) ] };
        char *ptr = buff.get();

        Serializable< Tup >::serialize( t, &ptr );
        assert_eq( buff.get() + size, ptr, "" );
        ptr = buff.get();
        Tup t2 = Serializable< Tup >::deserialize( &ptr );
        assert_eq( buff.get() + size, ptr, "" );
        assert_eq( t, t2, "" );
    }

    Test array() {
        using Type = std::array< int, 4 >;
        const int size = 4 * sizeof( int );

        Type arr{ { 1, 2, 3, 4 } };

        std::unique_ptr< char[] > buff{ new char[ sizeof( Type ) ] };
        char *ptr = buff.get();

        Serializable< Type >::serialize( arr, &ptr );
        assert_eq( buff.get() + size, ptr, "" );
        ptr = buff.get();
        Type arr2 = Serializable< Type >::deserialize( &ptr );
        assert_eq( buff.get() + size, ptr, "" );
        assert_eq( arr, arr2, "" );
    }
};

struct TestSerialization {

    struct _TestData {
        using Tuple = std::tuple< long, long, bool >;
        static TypeSignature type() { return TypeSignature::TestType; }
        static int size() { return sizeof( Tuple ); }
        Tuple tuple() const { return std::make_tuple( x, y, p ); }

        explicit _TestData( const Tuple &data ) :
            x( std::get< 0 >( data ) ), y( std::get< 1 >( data ) ),
            p( std::get< 2 >( data ) )
        { }

        _TestData( long x, long y, bool p ) : x( x ), y( y ), p( p ) { }
        _TestData() = default;

        long x, y;
        bool p;
    };

    Test typed() {
        _TestData data{ 0x7700ff770077ff00, 0x0077ff770077ff00, true };
        Serialized serial = Serializer::serialize( data );
        _TestData deser = Serializer::unsafeDeserialize< _TestData >( serial );
        assert_eq( data.x, deser.x, "serialization-deserialization error" );
        assert_eq( data.y, deser.y, "serialization-deserialization error" );
        assert_eq( data.p, deser.p, "serialization-deserialization error" );
    }

    Test packet() {
        _TestData data{ 0x7700ff770077ff00, 0x0077ff770077ff00, true };
        udp::Packet packet = Serializer::toPacket( data );
        _TestData deser = Serializer::unsafeFromPacket< _TestData >( packet );
        assert_eq( data.x, deser.x, "serialization-deserialization error" );
        assert_eq( data.y, deser.y, "serialization-deserialization error" );
        assert_eq( data.p, deser.p, "serialization-deserialization error" );
    }
};


