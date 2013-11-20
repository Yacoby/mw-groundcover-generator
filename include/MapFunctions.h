/*_LICENCE_BLOCK_
------------------------------------------------------------------
This source file is part of Morrowind Remake

Copyright (c) 2007 Jacob Essex
Also see acknowledgements in the readme

Morrowind Remake is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Morrowind Remake is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/gpl.txt

------------------------------------------------------------------
_LICENCE_BLOCK_*/
#ifndef _STL_MAP_FUNC_H_
#define _STL_MAP_FUNC_H_

#include <map>
/**
* Generates a 2d map, std::map<key, std::map<key, val> >
* Used as it also generates all the default settings. This means it can be used with map2dDoesExist()
*/
#define STD_MAP2d(key, val)						\
	std::map<									\
		key,									\
		std::map<key, val>,						\
		std::less<key>, 						\
		std::allocator<  						\
			std::pair< 							\
				const key, 						\
				std::map< 						\
					key, 						\
					val, 						\
					std::less<key>, 			\
					std::allocator< 			\
						std::pair< 				\
							const key, 			\
							val		  			\
						>						\
					>  							\
				>  						 		\
			> 									\
		> 										\
	>											

/**
* Checks if a item on a 2d map exists
* @param K the map key class
* @param V the map value class
* @param k1 the first key
* @param k1 the section key
* @param m the map
*/

template <class K, class V> inline bool map2dDoesExist(K k1, K k2, std::map< K, std::map<K, V> > &m ){
	if ( m.find(k1) != m.end() ){
		if ( m[k1].find(k2) != m[k1].end() )
			return true;
	}

	return false;
}

#endif

