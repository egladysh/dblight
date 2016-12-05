#ifndef HPP_BCE8C337DD384FBE851FD22347AB37DC
#define HPP_BCE8C337DD384FBE851FD22347AB37DC

#include <string>
#include <boost/noncopyable.hpp>
#include "sqlite3.h"


namespace dblight
{
	struct db : boost::noncopyable
	{
		typedef sqlite3* hnd;

		struct stm : boost::noncopyable
		{
			typedef sqlite3_stmt* hnd;

			stm( db& d, const std::string& stm );
			~stm();

			int count_cols() const
			{
				return ::sqlite3_column_count( h_ );
			}

			void bind( int i, const std::string& v );
			void bind( int i, int v );

			bool next_row();
			void commit();

			std::string get_text( int col ) const;
			int get_int( int col ) const;

		private:
			db::hnd dbh_;
			hnd h_;
			bool done_;
			std::string stm_;
		};

		db( const std::string& path );
		~db();

		bool is_table_exists( const std::string& name );

		hnd get_hnd() const
		{
			return h_;
		}

		struct transaction
		{
			db& db_;

			transaction( db& db )
				:db_(db)
				,fail_(false)
			{
				::sqlite3_exec(db_.get_hnd(), "BEGIN;", 0, 0, 0);
			}
			~transaction()
			{
				if( !fail_ )
					::sqlite3_exec(db_.get_hnd(), "END;", 0, 0, 0);
				else
					::sqlite3_exec(db_.get_hnd(), "ROLLBACK;", 0, 0, 0);
			}

			void fail()
			{
				fail_ = true;
			}
		private:
			bool fail_;
		};

	private:
		hnd h_;

	};
}

#endif
