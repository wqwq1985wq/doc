#pragma once
/*
 * 用途: 用户定出json对象结构 然后输出为json字串  (浮点和指数用字串吧,要求不要太高)
 */
#if 0
//目标是输出: (方便观看,所以没写键值的双引号,无需回车和对齐)
{
 a:
 {
    vec:   [1001, {sub:"9.99"}, ["热巴", "88.8"]],
    descp: "hahaha",
    prop:  {name:"levy", age:20}
 }
}
//希望代码是:
jsonWriter doc(buf, buflen);
jsonObject * root = doc.create_rootObject();
jsonObject * a  = root->addObjectVal("a");

jsonArray * vec = a->addArrayVal("vec")->push(new jsonNumber(1001))->push((new jsonObject()))->push(new jsonArray());
(*vec)[1]->addStringVal("sub", "9.99");
(*vec)[2]->push(new jsonString("热巴"))->push(new jsonString("88.8"));

a->addStringVal("descp", "hahaha");

jsonObject * prop = a->addObjectVal("prop");
prop->addStringVal("name", "levy");
prop->addNumberVal("age",20);

doc.writeTo(data, sizeof(data));
#endif

/*
 * 
 */
struct jsonValue
{
	enum VALTYPE { JSON_INVALID = 0, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };

	VALTYPE		m_type;

	union
	{
		int64		 m_lval;
		float		 m_fval;
		const char * m_str;
	};
	
	VALTYPE	 type() const { return m_type; }
	float operator (){ return m_fval; }
};

struct jsonNumber : jsonValue
{
	jsonNumber(int64 v):m_type(JSON_NUMBER)
	{
		m_lval = v;
	}
};

struct jsonString : jsonValue
{
	jsonString(const char * str):m_type(JSON_STRING)
	{
		m_str = str;
	}
};

class jsonArray : jsonValue
{
public:
	jsonArray():m_type(JSON_ARRAY)
	{
		
	}
	
	jsonArray * push(jsonValue *);   //返回this

	jsonValue * operator [](int index);  //第0~n-1个元素

	inline int size() const{ return m_vals.size(); }

public:
	std::vector<jsonValue *>	m_vals;
};

class jsonObject : jsonValue
{
public:
	jsonObject *  addObjectVal(const char * key);
	jsonArray *	  addArrayVal(const char * key);
	bool	addNumberVal(const char * key, int64 val);
	bool	addStringVal(const char * key, const char * val);

	inline int size() const{ return m_keys.size(); }

protected:
	std::vector<const char *>	m_keys;
	std::vector<jsonValue >	 m_vals;
};

/////
typedef struct 
{
	void* (*mem_alloc)(int size);
	void (*mem_free)(void *);

}writer_setting;

/*
 * Json输出者
 */
class jsonWriter
{
public:
	jsonWriter(char * buf, int buflen, writer_setting *);

	jsonObject * create_rootArray();

	jsonObject * create_rootObject();

	int		writeTo(char * buf, int maxlen);  //返回生成的完整json串的长度(包含0结尾)

protected:
	char *	m_buf;
	int		m_maxlen, m_uselen;
};
