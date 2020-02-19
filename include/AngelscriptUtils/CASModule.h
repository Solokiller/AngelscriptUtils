#ifndef ANGELSCRIPT_CASMODULE_H
#define ANGELSCRIPT_CASMODULE_H

#include <cstring>
#include <memory>

#include "ASUtilsConfig.h"

#include "AngelscriptUtils/utility/BaseClasses.h"

class asIScriptModule;

namespace asutils
{
class Scheduler;
}

/**
*	@defgroup ASModule Angelscript Module
*
*	@{
*/

/**
*	Angelscript module. Wraps asIScriptModule and provides the descriptor.
*	Is reference counted, unlike the script module.
*/
class CASModule final : public asutils::AtomicReferenceCountedClass
{
public:
	/**
	*	Constructor.
	*	@param pModule Script module.
	*/
	CASModule( asIScriptModule* pModule );

	/**
	*	Destructor.
	*/
	~CASModule();

	void Release() const;

	/**
	*	Discards the module. It can no longer be used after this.
	*/
	void Discard();

	/**
	*	@return The script module.
	*/
	asIScriptModule* GetModule() { return m_pModule; }

	/**
	*	@return The module name.
	*/
	const char* GetModuleName() const;

	/**
	*	@return The scheduler.
	*/
	asutils::Scheduler& GetScheduler() { return *m_Scheduler; }

private:
	asIScriptModule* m_pModule;

	std::unique_ptr<asutils::Scheduler> m_Scheduler;

private:
	CASModule( const CASModule& ) = delete;
	CASModule& operator=( const CASModule& ) = delete;
};

/**
*	Gets a module from a script module.
*	@param pModule Script module to retrieve the module from.
*	@return The module, or null if it couldn't be retrieved.
*/
CASModule* GetModuleFromScriptModule( const asIScriptModule* pModule );

/**
*	Gets a module from a script function.
*	@param pFunction Script function to retrieve the module from.
*	@return The module, or null if it couldn't be retrieved.
*/
CASModule* GetModuleFromScriptFunction( const asIScriptFunction* pFunction );

/**
*	Gets a module from a script context.
*	@param pContext Script context to retrieve the module from. Uses the function currently being executed.
*	@return The module, or null if it couldn't be retrieved.
*/
CASModule* GetModuleFromScriptContext( asIScriptContext* pContext );

/**
*	Gets a script module from a script context.
*	@param pContext Script context to retrieve the module from. Uses the function currently being executed.
*	@return The script module, or null if it couldn't be retrieved.
*/
asIScriptModule* GetScriptModuleFromScriptContext( asIScriptContext* pContext );

/**
*	Less function for modules.
*	@param pLHS Left hand module.
*	@param pRHS Right hand module.
*	@return true if the left hand module is smaller than the right hand module.
*/
inline bool ModuleLess( const CASModule* pLHS, const CASModule* pRHS )
{
	//Use the memory address for sorting. They need only be sorted in a unique order.
	return pLHS < pRHS;
}

/**
*	Functor that overloads operator() to return true if a module equals a given name.
*/
struct ModuleEqualByName final
{
	const char* const pszModuleName;

	ModuleEqualByName( const char* const pszModuleName )
		: pszModuleName( pszModuleName )
	{
	}

	ModuleEqualByName( const ModuleEqualByName& other ) = default;

	bool operator()( const CASModule* pModule ) const
	{
		return strcmp( pModule->GetModuleName(), pszModuleName ) == 0;
	}

private:
	ModuleEqualByName& operator=( const ModuleEqualByName& ) = delete;
};

/** @} */

#endif //ANGELSCRIPT_CASMODULE_H