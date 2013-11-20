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
/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the NIF File Format Library and Tools
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE. */

#ifndef _NIFLIB_REF_H_
#define _NIFLIB_REF_H_

#include <ostream>


using namespace std;


namespace ES3{


/**
 * Smart Pointer Template
 */

template<class T> class ESRef;
template<class T> ostream & operator<<(ostream &, const ESRef<T> &);

template <class T> class ESRef {
public:
	ESRef( T * object = NULL );
	ESRef(const ESRef & ref_to_copy );

	~ESRef();

	T& operator*() const;

	bool operator<(const ESRef & ref) const;

	bool operator==(T * object) const;
	bool operator!=(T * object) const;
	bool operator==(const ESRef & ref) const;
	bool operator!=(const ESRef & ref) const;

   friend ostream & operator<< <T>(ostream & os, const ESRef & ref);
	ESRef & operator=( T * object );
	ESRef & operator=( const ESRef & ref );
	operator T*() const;
	T* operator->() const;

protected:
	//The shared object
	T* _object;
};


}//namespace


using namespace ES3;


template <class T>
ESRef<T>::ESRef( T * object ) : _object(object) {
   //If object isn't null, increment reference count
   if ( _object != NULL ) {
      _object->ref_addESRef();
   }
}

template <class T>
ESRef<T>::ESRef(const ESRef & ref_to_copy ) {
	_object = ref_to_copy._object;
	//If object isn't null, increment reference count
	if ( _object != NULL ) {
		_object->ref_addESRef();
	}
}

template <class T>
ESRef<T>::~ESRef() {
	//if object insn't null, decrement reference count
	if ( _object != NULL ) {
		_object->ref_subtractESRef();
	}
}

template <class T>
ESRef<T>::operator T*() const {
	return _object;
}



template <class T>
T* ESRef<T>::operator->() const {
	return _object;
}

template <class T>
T& ESRef<T>::operator*() const {
	return *_object;
}

template <class T>
ESRef<T> & ESRef<T>::operator=( T * object ) {
	//Check if referenced objects are already the same
	if ( _object == object ) {
		return *this; //Do nothing
	}

	//Increment reference count on new object if it is not NULL
	if ( object != NULL ) {
		object->ref_addESRef();
	}

	//Decriment reference count on previously referenced object, if any
	if ( _object != NULL ) {
		_object->ref_subtractESRef();
	}

	//Change reference to new object
	_object = object;

	return *this;
}


template <class T>
ESRef<T> & ESRef<T>::operator=( const ESRef & ref ) {
	//Check if referenced objects are already the same
	if ( _object == ref._object ) {
		return *this; //Do nothing
	}

	//Increment reference count on new object if it is not NULL
	if ( ref._object != NULL ) {
		ref._object->ref_addESRef();
	}

	//Decriment reference count on previously referenced object, if any
	if ( _object != NULL ) {
		_object->ref_subtractESRef();
	}

	//Change reference to new object
	_object = ref._object;

	return *this;
}

//Template functions must be in the header file

template <class T>
bool ESRef<T>::operator<(const ESRef & ref) const {
	return (_object < ref._object);
}

template <class T>
bool ESRef<T>::operator==(T * object) const {
	//Compare pointer values of referenced objects
	return ( _object == object );
}

template <class T>
bool ESRef<T>::operator!=(T * object) const {
	//Compare pointer values of referenced objects
	return ( _object != object );
}

template <class T>
bool ESRef<T>::operator==(const ESRef & ref) const {
	//Compare pointer values of referenced objects
	return ( _object == ref._object );
}

template <class T>
bool ESRef<T>::operator!=(const ESRef & ref) const {
	//Compare pointer values of referenced objects
	return ( _object != ref._object );
}

template <class T>
ostream & operator<<(ostream & os, const ESRef<T> & ref) {
	if (ref._object) {
		os << ref->GetIDString();
	} else {
		os << "NULL";
	}
	return os;
}
#endif
