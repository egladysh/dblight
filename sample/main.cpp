#include <iostream>
#include <sstream>
#include "dblight.hpp"

int main()
{
	#if defined(_MSC_VER)
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	std::cout << "dblight sample" << std::endl;

	dblight::db db("./dblight_sample_db.bin"); //path to the db file

	try
	{
		if( !db.is_table_exists( "test_data" ) )
		{
			{
				dblight::db::stm q( 
						db
						,"CREATE TABLE test_data "
						"("
						"  id INT"
						", text CHAR(64)"
						")"
						);
				q.commit();
			}
			{
				dblight::db::stm q( 
						db
						,"CREATE UNIQUE INDEX test_id ON test_data(id)"
						);
				q.commit();
			}
		}
	}
	catch( const std::exception& e)
	{
		std::cout << "create error: " << e.what() << std::endl;
		return 1;
	}

	try
	{
		std::cout << "inserting..." << std::endl;
		dblight::db::stm q( 
				db
				,"INSERT INTO test_data VALUES(?,?)"
			);
		q.bind(0, 1);
		q.bind(1, "hello dblight");
		q.commit();
	}
	catch( const std::exception& e)
	{
		std::cout << "insert error: " << e.what() << std::endl;
	}

	try
	{
		std::cout << "selecting..." << std::endl;
		dblight::db::stm q( 
				db
				,"SELECT * FROM test_data"
			);

		while( q.next_row() )
		{
			std::cout << "id=" << q.get_int(0) << " text=" << q.get_text(1) << std::endl;
		}
	}
	catch( const std::exception& e)
	{
		std::cout << "select error: " << e.what() << std::endl;
	}

	return 0;
}
