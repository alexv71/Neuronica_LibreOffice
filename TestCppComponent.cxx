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

#include <stdio.h>
#include <sal/main.h>
#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <neuronica/BackProp.hpp>

using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::frame;
using namespace std;

using ::rtl::OUString;
using ::rtl::OUStringToOString;

SAL_IMPLEMENT_MAIN()
{
    try
    {
		short res1;
		long res2;
		long res3;
		
		// get the remote office component context
        Reference< XComponentContext > xContext( ::cppu::bootstrap() );
        fprintf(stdout, "\nconnected to a running office...\n");

        // create a new instance of BackProp with the specified string argument
        Reference<neuronica::XBackProp> xNN = neuronica::BackProp::create(xContext, OUString("Hello My World!"));

        // call methodTwo and print the return value of methodTwo
//        OUString s = xSomething->methodTwo();
		res1 = xNN->createNetwork3(3,2,1);
		res1 = xNN->createNetwork3(2,2,1);
		res2 = xNN->getLayersCount();
		res3 = xNN->getWeightsCount();

        fprintf(stdout, "\nCreate new instance of BackProp with %d result", res1);
        fprintf(stdout, "\nThe new neural network has %d layers", res2);
        fprintf(stdout, "\nThe new neural network has %d weights", res3);

		xNN->createPatterns(4);
		xNN->setPatternValue(0, 0, 0.00);
		xNN->setPatternValue(0, 1, 0.00);
		xNN->setPatternValue(0, 2, 0.00);

		xNN->setPatternValue(1, 0, 0.00);
		xNN->setPatternValue(1, 1, 1.00);
		xNN->setPatternValue(1, 2, 1.00);

		xNN->setPatternValue(2, 0, 1.00);
		xNN->setPatternValue(2, 1, 0.00);
		xNN->setPatternValue(2, 2, 1.00);

		xNN->setPatternValue(3, 0, 1.00);
		xNN->setPatternValue(3, 1, 1.00);
		xNN->setPatternValue(3, 2, 0.00);

		xNN->initializeNetwork(3);
        fprintf(stdout, "\nStarting train...");
		xNN->trainBPROP(15000, 0.001, 0.5, 0.1);

		xNN->test(0); fprintf(stdout, "\n%f", xNN->getActivation(4));
		xNN->test(1); fprintf(stdout, "\n%f", xNN->getActivation(4));
		xNN->test(2); fprintf(stdout, "\n%f", xNN->getActivation(4));
		xNN->test(3); fprintf(stdout, "\n%f", xNN->getActivation(4));
		
		fprintf(stdout, "\n\nPlease press 'return' to finish the example!\n");
        getchar();
    }
    catch ( ::cppu::BootstrapException & e )
    {
        fprintf(stderr, "\ncaught BootstrapException: %s\n",
                OUStringToOString( e.getMessage(), RTL_TEXTENCODING_ASCII_US ).getStr());
        return 1;
    }
    catch ( Exception & e )
    {
        fprintf(stderr, "\ncaught UNO exception: %s\n",
                OUStringToOString( e.Message, RTL_TEXTENCODING_ASCII_US ).getStr());
        return 1;
    }

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
