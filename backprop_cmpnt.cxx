/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*************************************************************************
 *
 *  The Contents of this file are made available subject to the terms of
 *  the BSD license.
 *
 *  Copyright 2000, 2010 Oracle and/or its affiliates.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Sun Microsystems, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

#include <osl/interlck.h>
#include <osl/mutex.hxx>
#include <cppuhelper/factory.hxx>
#include <cppuhelper/supportsservice.hxx>
#include <cppuhelper/implbase3.hxx>
#include <cppuhelper/implementationentry.hxx>
#include <cppuhelper/supportsservice.hxx>
#include <uno/lbnames.h>

#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/XTypeProvider.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/lang/IllegalArgumentException.hpp>
#include <neuronica/XBackProp.hpp>
#include "neuralnet.h"

using namespace ::rtl; // for OUString
using namespace ::com::sun::star; // for odk interfaces
using namespace ::com::sun::star::uno; // for basic types

namespace my_sc_impl
{

static Sequence< OUString > getSupportedServiceNames_BackPropImpl()
{
    Sequence<OUString> names(1);
    names[0] = OUString("neuronica.BackProp");
    return names;
}

static OUString getImplementationName_BackPropImpl()
{
    return OUString("neuronica.my_sc_implementation.BackProp");
}

class BackPropImpl  
	: public ::neuronica::XBackProp
	, public lang::XServiceInfo
	, public lang::XTypeProvider
	, public NeuralNet
//
//	public ::cppu::WeakImplHelper3< ::neuronica::XBackProp, lang::XServiceInfo, lang::XInitialization >, public NeuralNet
{
	oslInterlockedCount m_refcount;
	OUString m_sData;

//	NeuralNet *m_NNet;
    // it's good practise to store the context for further use when you use
    // other UNO API's in your implementation
    Reference< XComponentContext > m_xContext;
public:
	inline BackPropImpl(Reference< XComponentContext > const & xContext) throw ()
		: m_refcount(0),
		m_xContext(xContext) {}
	
	virtual ~BackPropImpl() {}

	// XInterface
	virtual Any SAL_CALL queryInterface(Type const & type)
		throw (RuntimeException);
	virtual void SAL_CALL acquire()
		throw ();
	virtual void SAL_CALL release()
		throw ();

	// XTypeProvider
	virtual Sequence< Type > SAL_CALL getTypes()
		throw (RuntimeException);
	virtual Sequence< sal_Int8 > SAL_CALL getImplementationId()
		throw (RuntimeException);

	// XBackProp
    virtual sal_Int16 SAL_CALL createNetwork3(sal_Int32 il, sal_Int32 hl, sal_Int32 ol)
        throw (RuntimeException);
    virtual void SAL_CALL initializeNetwork(sal_Int32 randomSeed)
        throw (RuntimeException);
    virtual sal_Int32 SAL_CALL getLayersCount()
        throw (RuntimeException);
    virtual sal_Int32 SAL_CALL getWeightsCount()
        throw (RuntimeException);

	virtual sal_Int32 SAL_CALL getLayer(sal_Int32 nLayer)
        throw (RuntimeException);
	virtual double SAL_CALL getWeight(sal_Int32 nWeight)
        throw (RuntimeException);
	virtual void SAL_CALL setWeight(sal_Int32 nWeight, double nValue)
        throw (RuntimeException);

	virtual void SAL_CALL createPatterns(sal_Int32 nPatterns)
        throw (RuntimeException);
	virtual double SAL_CALL getPatternValue(sal_Int32 nPattern, sal_Int32 nUnit)
        throw (RuntimeException);
	virtual void SAL_CALL setPatternValue(sal_Int32 nPattern, sal_Int32 nUnit, double nValue)
        throw (RuntimeException);

	virtual void SAL_CALL trainBPROP(sal_Int32 Length, double MaxError, double LearningRate, double Momentum)
        throw (RuntimeException);
	virtual void SAL_CALL trainRPROP(sal_Int32 Length, double MaxError, double IncreaseFactor, double DecreaseFactor, double DeltaMin, double DeltaMax, double DeltaInit)
        throw (RuntimeException);
	virtual void SAL_CALL trainSCG(sal_Int32 Length, double MaxError, double Sigma, double Lambda)
        throw (RuntimeException);
	virtual void SAL_CALL test(sal_Int32 nPattern)
        throw (RuntimeException);
	virtual double SAL_CALL getActivation(sal_Int32 nIndex)
        throw (RuntimeException);

	// XServiceInfo
	virtual OUString SAL_CALL getImplementationName()
		throw (RuntimeException);
	virtual sal_Bool SAL_CALL supportsService(OUString const & serviceName)
		throw (RuntimeException);
	virtual Sequence< OUString > SAL_CALL getSupportedServiceNames()
		throw (RuntimeException);
};


// XInterface implementation
Any BackPropImpl::queryInterface(Type const & type)
throw (RuntimeException)
{
	if (type.equals(::cppu::UnoType<XInterface>::get()))
	{
		// return XInterface interface
		// (resolve ambiguity by casting to lang::XTypeProvider)
		Reference< XInterface > x(
			static_cast< lang::XTypeProvider * >(this));
		return makeAny(x);
	}
	if (type.equals(::cppu::UnoType<lang::XTypeProvider>::get()))
	{
		// return XInterface interface
		Reference< XInterface > x(
			static_cast< lang::XTypeProvider * >(this));
		return makeAny(x);
	}
	if (type.equals(::cppu::UnoType<lang::XServiceInfo>::get()))
	{
		// return XServiceInfo interface
		Reference< lang::XServiceInfo > x(
			static_cast< lang::XServiceInfo * >(this));
		return makeAny(x);
	}
	if (type.equals(::cppu::UnoType<neuronica::XBackProp>::get()))
	{
		// return sample interface
		Reference< ::neuronica::XBackProp > x(
			static_cast< ::neuronica::XBackProp * >(this));
		return makeAny(x);
	}
	// querying for unsupported type
	return Any();
}

void BackPropImpl::acquire()
throw ()
{
	// thread-safe incrementation of reference count
	::osl_atomic_increment(&m_refcount);
}

void BackPropImpl::release()
throw ()
{
	// thread-safe decrementation of reference count
	if (0 == ::osl_atomic_decrement(&m_refcount))
	{
		delete this; // shutdown this object
	}
}

// XTypeProvider implementation
Sequence< Type > BackPropImpl::getTypes()
throw (RuntimeException)
{
	Sequence< Type > seq(3);
	seq[0] = ::cppu::UnoType<lang::XTypeProvider>::get();
	seq[1] = ::cppu::UnoType<lang::XServiceInfo>::get();
	seq[2] = ::cppu::UnoType<neuronica::XBackProp>::get();
	return seq;
}

Sequence< sal_Int8 > BackPropImpl::getImplementationId()
throw (RuntimeException)
{
	return css::uno::Sequence<sal_Int8>();
}

// XBackProp implementation //================================================================
sal_Int16 BackPropImpl::createNetwork3(sal_Int32 il, sal_Int32 hl, sal_Int32 ol)
    throw (RuntimeException)
{
	long i_array[3];
	sal_Int16 result=0;
	i_array[0] = il; i_array[1] = hl; i_array[2] = ol;
	result = CreateNetwork(3, i_array);
//	delete i_array;
	
	return result;
}

void BackPropImpl::initializeNetwork(sal_Int32 randomSeed)
    throw (RuntimeException)
{
	InitializeNetwork(randomSeed);
	return;
}

sal_Int32 BackPropImpl::getLayersCount()
    throw (RuntimeException)
{
	return (sal_Int32) m_lLayersCount;
}

sal_Int32 BackPropImpl::getWeightsCount()
    throw (RuntimeException)
{
	return (sal_Int32) m_lWeightsCount;
}

sal_Int32 BackPropImpl::getLayer(sal_Int32 nLayer)
{
	return (sal_Int32) m_plLayers[nLayer];
}

double BackPropImpl::getWeight(sal_Int32 nWeight)
{
	return m_pdWeights[nWeight];
}

void BackPropImpl::setWeight(sal_Int32 nWeight, double nValue)
{
	m_pdWeights[nWeight] = nValue;
}

void BackPropImpl::createPatterns(sal_Int32 nPatterns)
{
	CreatePatterns(nPatterns);
}

double BackPropImpl::getPatternValue(sal_Int32 nPattern, sal_Int32 nUnit)
{
	return m_pdPatterns[(m_plLayers[0] + m_plLayers[m_lLayersCount - 1]) * nPattern + nUnit];
}

void BackPropImpl::setPatternValue(sal_Int32 nPattern, sal_Int32 nUnit, double nValue)
{
	SetPattern(nPattern, nUnit, nValue);
}

void BackPropImpl::trainBPROP(sal_Int32 Length, double MaxError, double LearningRate, double Momentum)
{
	TrainBPROP(Length, MaxError, LearningRate, Momentum);
}

void BackPropImpl::trainRPROP(sal_Int32 Length, double MaxError, double IncreaseFactor, double DecreaseFactor, double DeltaMin, double DeltaMax, double DeltaInit)
{
	TrainRPROP(Length, MaxError, IncreaseFactor, DecreaseFactor, DeltaMin, DeltaMax, DeltaInit);
}

void BackPropImpl::trainSCG(sal_Int32 Length, double MaxError, double Sigma, double Lambda)
{
	TrainSCG(Length, MaxError, Sigma, Lambda);
}

void BackPropImpl::test(sal_Int32 nPattern)
{
	Test(nPattern);
}

double BackPropImpl::getActivation(sal_Int32 nIndex)
{
	if (nIndex >=0 && nIndex < m_lNodesCount)
		return m_pdNodes[nIndex];
	else
		return -1; 
}

// XServiceInfo implementation
OUString BackPropImpl::getImplementationName()
throw (RuntimeException)
{
	// unique implementation name
	return OUString("neuronica.my_sc_implementation.BackProp");
}
sal_Bool BackPropImpl::supportsService(OUString const & serviceName)
throw (RuntimeException)
{
	return cppu::supportsService(this, serviceName);
}
Sequence< OUString > BackPropImpl::getSupportedServiceNames()
throw (RuntimeException)
{
	// this object only supports one service
	OUString serviceName("neuronica.BackProp");
	return Sequence< OUString >(&serviceName, 1);
}

//////
Reference< XInterface > SAL_CALL create_BackPropImpl(
	Reference< XComponentContext > const & xContext)
{
	return static_cast< lang::XTypeProvider * >(new BackPropImpl(xContext));
}


}

/* shared lib exports implemented without helpers in service_impl1.cxx */
namespace my_sc_impl
{
	static const struct ::cppu::ImplementationEntry s_component_entries[] =
	{
		{
			create_BackPropImpl, getImplementationName_BackPropImpl,
			getSupportedServiceNames_BackPropImpl,
			::cppu::createSingleComponentFactory,
		0, 0
		},
		{ 0, 0, 0, 0, 0, 0 }
	};
}

extern "C"
{

SAL_DLLPUBLIC_EXPORT void * SAL_CALL component_getFactory(
    sal_Char const * implName, lang::XMultiServiceFactory * xMgr,
    registry::XRegistryKey * xRegistry )
{
    return ::cppu::component_getFactoryHelper(
        implName, xMgr, xRegistry, ::my_sc_impl::s_component_entries );
}

SAL_DLLPUBLIC_EXPORT void SAL_CALL component_getImplementationEnvironment(
    char const ** ppEnvTypeName, uno_Environment **)
{
    *ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
}

}


/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
