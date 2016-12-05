
#include "dblight.hpp"
#include <stdexcept>
#include "boost/thread/thread.hpp"

using namespace dblight;

db::stm::stm( db& d, const std::string& stm )
	:dbh_(d.get_hnd())
	,h_(nullptr)
	,done_(false)
	,stm_(stm)
{
	int c = ::sqlite3_prepare_v2(dbh_, stm_.c_str(), (int)stm_.length(), &h_, NULL);
	if( c !=  SQLITE_OK )
	{
		const char* msg = sqlite3_errmsg(dbh_);
		throw std::runtime_error(std::string("unable to prepare statement: ") + (msg?msg:""));
	}
}

db::stm::~stm()
{
	::sqlite3_finalize( h_ );
}

void db::stm::bind( int i, const std::string& v )
{
	int c = ::sqlite3_bind_text(h_, i+1, v.c_str(), -1, SQLITE_TRANSIENT);
	if( c !=  SQLITE_OK )
	{
		throw std::runtime_error("db: unable to bind");
	}
}
void db::stm::bind( int i, int v )
{
	int c = ::sqlite3_bind_int(h_, i+1, v );
	if( c !=  SQLITE_OK )
	{
		throw std::runtime_error("db: unable to bind");
	}
}
static int MAX_DB_RETRY = 4;
void db::stm::commit()
{
	int cnt = 0;
	int c = 0;
	while(true)
	{
		try
		{
			c = ::sqlite3_step(h_);
		}
		catch( const std::exception& e )
		{
			c = SQLITE_BUSY;
		}
		if( c == SQLITE_DONE )
			break;

		const char* msg = sqlite3_errmsg(dbh_);
		if( c == SQLITE_BUSY && ++cnt < MAX_DB_RETRY)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(40));
			continue;
		}
		throw std::runtime_error(std::string("unable to exec statement: ") + (msg?msg:""));
	}
}

bool db::stm::next_row()
{
	if( done_ )
		return false;

	int cnt = 0;
	int c = 0;
	while(true)
	{
		try
		{
			c = ::sqlite3_step(h_);
		}
		catch( const std::exception& e )
		{
			c = SQLITE_BUSY;
		}
		if( c == SQLITE_ROW )
			return true;

		if( c == SQLITE_DONE )
		{
			done_ = true;
			return false;
		}
		const char* msg = sqlite3_errmsg(dbh_);
		if( c == SQLITE_BUSY && ++cnt < MAX_DB_RETRY)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(40));
			continue;
		}
		throw std::runtime_error(std::string("unable to exec statement: ") + (msg?msg:""));
	}
	return false;
}

std::string db::stm::get_text( int col ) const
{
	const unsigned char* b = ::sqlite3_column_text( h_, col );
	if( !b )
		return std::string();
	return std::string( (const char*)b, (const char*)b+strlen((const char*)b) );
}
int db::stm::get_int( int col ) const
{
	return ::sqlite3_column_int( h_, col );
}

db::db( const std::string& path )
	:h_(nullptr)
{
	int c = ::sqlite3_open( path.c_str(), &h_ );
	if( c !=  SQLITE_OK )
	{
		throw std::runtime_error("unable to open the database");
	}
}
db::~db()
{
	::sqlite3_close( h_ );
}

bool db::is_table_exists( const std::string& name )
{
	try
	{
		stm query( *this, std::string("SELECT * from ") + name + " LIMIT 1" );
		return true;
	}
	catch( const std::exception& )
	{
		return false;
	}
}





