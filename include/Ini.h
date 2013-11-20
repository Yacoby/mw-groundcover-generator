#ifndef __INI_H_
#define __INI_H_

class Ini{

	std::string mCurBlok;
protected:
	typedef std::map<std::string, std::map<std::string, std::string> > IniValues;
	typedef std::map<std::string, std::map<std::string, std::string> >::iterator IniValuesItr;
	IniValues mValues;
	virtual void afterLoad(){}
public:
	Ini():mCurBlok("unknown"){}

	void deleteCat(const std::string& cat){
		mValues.erase(cat);	
	}


	bool catExists(const std::string& cat){
		return mValues.find(cat) != mValues.end() ? true : false; 
	}

	bool valueExists(const std::string& cat, const std::string& key){
		if ( !catExists(cat)) return false;
		return mValues[cat].find(key) != mValues[cat].end() ? true : false; 
	}

	std::string getValue(const std::string& cat, const std::string& key){
		if ( !valueExists(cat, key) ) throw std::exception (std::string("Value doesn't exist\n" + cat + " - " + key).c_str());
		return mValues[cat][key];
	}
	void setValue(const std::string& cat, const std::string& key, const std::string& val){
		mValues[cat][key] = val;
	}

	bool save(const std::string& f){
		std::ofstream ofs(f.c_str());
		if ( !ofs.is_open() ) return false;

		for ( IniValuesItr iter1 = mValues.begin(); iter1 != mValues.end(); ++iter1){
			ofs << "[" << iter1->first << "]\n";
			
			for (  std::map<std::string, std::string>::iterator iter2= iter1->second.begin();
																iter2 != iter1->second.end();
																++iter2){
				ofs << iter2->first << "=" << iter2->second << "\n";
			}
		}
		ofs.close();
		return true;
	}


	bool load(const std::string& f){
		std::ifstream ifs(f.c_str());
		if ( !ifs.is_open() ) return false;


		std::string line, key, val;
		while ( !ifs.eof() ){
			std::getline(ifs, line);

			if ( line.find(";") != -1 ) continue; //comment


			if ( line.find("[") == 0 ){
				mCurBlok = line.substr(1, line.length());
				mCurBlok = mCurBlok.substr(0, line.length()-2);
				continue;
			}


			size_t pos = line.find("=");
			if ( pos == -1 ) continue; //not a valid value

			key = line.substr(0, pos);
			val = line.substr(pos+1, line.length()-pos+1);

			mValues[mCurBlok][key] = val;
		}
		ifs.close();
		return true;
	}
};

class GrassIni2 : public Ini{
public:
	struct GrassMesh{
		std::string mesh;
		std::string objectID;
		float chance;
		int id;
	};
public: 

	/**
	* gets a list of all the texutes used
	*/
	std::list<std::string> getTextureList(){
		std::list<std::string> lst;
		for ( IniValuesItr iter = mValues.begin(); iter != mValues.end(); ++iter)
			lst.push_back(iter->first);
		return lst;
	}

	/**
	* @brief gets a list of meshes used by a texture
	*/
	std::list<GrassMesh> getMeshList(const std::string& cat){
		std::list<GrassMesh> lst;
		if ( !catExists(cat) ) return lst;
		for ( int c = 0;valueExists(cat, "sMesh"+toString(c));++c ){
			GrassMesh g;
			g.mesh = getValue(cat, "sMesh"+toString(c));
			g.chance = fromString<float>(getValue(cat, "sChance"+toString(c)));
			if ( valueExists(cat, "sID"+toString(c)) )
				g.objectID = getValue(cat, "sID"+toString(c));
			g.id = c;
			lst.push_back(g);
		}
		return lst;
	}

	bool randomScaleSettings(const std::string& cat, float& min, float& max){
		if ( !catExists(cat) ) throw std::exception("The category doesn't exist");

		if ( !valueExists(cat, "fSclMin")) throw std::exception("The value \"fSclMin\" doesn't exist");
		if ( !valueExists(cat, "fSclMax")) throw std::exception("The value \"fSclMax\" doesn't exist");
		if ( !valueExists(cat, "bSclRand")) throw std::exception("The value \"bSclRand\" doesn't exist");


		min = fromString<float>(getValue(cat, "fSclMin"));
		max = fromString<float>(getValue(cat, "fSclMax"));
		return fromString<bool>(getValue(cat, "bSclRand"));
	}
	bool randomPosSettings(const std::string& cat, float& min, float& max){
		if ( !catExists(cat) ) throw std::exception("The category doesn't exist");

		if ( !valueExists(cat, "fPosMin")) throw std::exception("The value \"fPosMin\" doesn't exist");
		if ( !valueExists(cat, "fPosMax")) throw std::exception("The value \"fPosMax\" doesn't exist");
		if ( !valueExists(cat, "bPosRand")) throw std::exception("The value \"bPosRand\" doesn't exist");

		min = fromString<float>(getValue(cat, "fPosMin"));
		max = fromString<float>(getValue(cat, "fPosMax"));
		return fromString<bool>(getValue(cat, "bPosRand"));
	}


};

class GrassIni{
public:

	struct Settings{
		int gap;
		float scale;
		std::string objectID;
		bool  posRnd;
		int posRndMin;
		int posRndMax;
		float offsetZ;

		bool  sclRnd;
		float sclRndMin;
		float sclRndMax;

		bool  rotRnd;
		float rotRndMin;
		float rotRndMax;

		std::string mesh;
	};


	typedef std::map<std::string, GrassIni::Settings> SettingsMap;
	typedef GrassIni::SettingsMap::iterator SettingsMapItr;

	GrassIni():mCurBlok("unknown"){}

	bool catExists(const std::string& cat){
		return mSettings.find(cat) != mSettings.end() ? true : false; 
	}

	Settings getSettings(const std::string& cat){
		SettingsMapItr itr = mSettings.find(cat);
		if ( itr == mSettings.end() ) assert(0);
		return itr->second;
	}

	SettingsMap& getSettings(){
		return mSettings;
	}

	void load(const std::string& f){
		std::ifstream ifs(f.c_str());


		std::string line, key, val;
		while ( !ifs.eof() ){
			std::getline(ifs, line);

			if ( line.find(";") != -1 ) continue; //comment


			if ( line.find("[") == 0 ){
				mCurBlok = line.substr(1, line.length());
				mCurBlok = mCurBlok.substr(0, line.length()-2);
				continue;
			}


			size_t pos = line.find("=");
			if ( pos == -1 ) continue; //not a valid value

			key = line.substr(0, pos);
			val = line.substr(pos+1, line.length()-pos+1);

			if ( key == "objid"){
				mSettings[mCurBlok].objectID = val;
			}else if ( key == "scale" ){
				mSettings[mCurBlok].scale = fromString<float>(val);
			}else if ( key == "gap" ){
				mSettings[mCurBlok].gap = fromString<int>(val);
			}else if ( key == "posrnd" ){
				mSettings[mCurBlok].posRnd = fromString<bool>(val);
			}else if ( key == "poslow" ){
				mSettings[mCurBlok].posRndMin = fromString<int>(val);
			}else if ( key == "poshigh" ){
				mSettings[mCurBlok].posRndMax = fromString<int>(val);
			}else if ( key == "sclrnd" ){
				mSettings[mCurBlok].sclRnd = fromString<bool>(val);
			}else if ( key == "scllow" ){
				mSettings[mCurBlok].sclRndMin = fromString<float>(val);
			}else if ( key == "sclhigh" ){
				mSettings[mCurBlok].sclRndMax = fromString<float>(val);
			}else if ( key == "rotrnd" ){
				mSettings[mCurBlok].rotRnd = fromString<bool>(val);
			}else if ( key == "rotlow" ){
				mSettings[mCurBlok].rotRndMin = fromString<float>(val);
			}else if ( key == "rothigh" ){
				mSettings[mCurBlok].rotRndMax = fromString<float>(val);
			}else if ( key == "offz" ){
				mSettings[mCurBlok].offsetZ = fromString<float>(val);
			}else if ( key == "mesh" ){
				mSettings[mCurBlok].mesh = val;
			}

		}//while ( !ifs.eof() ){


	}//void load(const std::string& f){


private:


	SettingsMap mSettings;

	std::string mCurBlok;

	template<typename T> T fromString( const std::string& s){
		std::istringstream is(s);
		T t;
		is >> t;
		return t;
	}

};


#endif //__INI_H_