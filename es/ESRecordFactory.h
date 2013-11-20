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
#ifndef _ESRECORDFACTORY_H_
#define _ESRECORDFACTORY_H_

#include <string>
#include <map>


namespace ES3{

class ESRecord;


/**
* Marco function to resgister a record
* @param classType The class name
* @param id The id of the class (i.e. STAT)
*/
#define REGISTER_RECORD(classType, id) ES3::ESRecordFactory::getInstance()->registerType<classType>(id);


/**
* Creates a new object of ClassType
*/
template<typename ClassType>
ESRecord* createObject(){
   return new ClassType();
}

/**
* Used for creating esmp record classes.
* When creating a new recorder, use the Macro REGISTER_RECORD(classType, id)
* \todo Make full singleton... http://www.codeguru.com/forum/showthread.php?t=344782
*/
class ESRecordFactory {

protected:
	typedef ESRecord *(*CreateObjectFunc)();
	
public:
	ESRecordFactory(){}
	~ESRecordFactory(){}

	
	/**
	* Retuens the instance to the class, as there should really only be one of these (totaly pointless having two...)
	*/
	static ESRecordFactory* getInstance(){
		static ESRecordFactory singleton;
		return &singleton;
	}

	
	typedef std::map<std::string, CreateObjectFunc>::const_iterator ConstIterator;              
	typedef std::map<std::string, CreateObjectFunc>::iterator Iterator;     


	/**
	* Resiters a new record type and class
	* @param id the unqiue id of the object
	* @return false if a obj with id already exists
	*/
	template<typename ClassType>
	bool registerType(std::string id){
		if (mObjectCreator.find(id) != mObjectCreator.end())
			return false;

		mObjectCreator[id] = &createObject<ClassType>;
		return true;
	}

	/**
	* unresigress a recored type (Why you would want to do this, IDK)
	*/
	bool unregisterType(std::string id);

	/**
	* Creates a record with the given id
	*/
	ESRecord* create(std::string id);

	
   ConstIterator GetBegin() const
   {
      return mObjectCreator.begin();
   }

   Iterator GetBegin()
   {
      return mObjectCreator.begin();
   }

   ConstIterator GetEnd() const
   {
      return mObjectCreator.end();
   }

   Iterator GetEnd()
   {
      return mObjectCreator.end();
   }


	
protected:
	
	std::map<std::string, CreateObjectFunc> mObjectCreator;

};

}//namespace

#endif
