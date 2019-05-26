
#ifndef HEADER_GUARD_BASE_EXCEPTION_H
#define HEADER_GUARD_BASE_EXCEPTION_H



class base_exception
{
public:

	base_exception() { }
	explicit base_exception(const std::string & ErrorMsg) : m_ErrorMsg(ErrorMsg) { }
	virtual ~base_exception() { }

	virtual std::string what() const { return m_ErrorMsg; }

protected:
	std::string		m_ErrorMsg;
};



#endif // HEADER_GUARD_BASE_EXCEPTION_H
