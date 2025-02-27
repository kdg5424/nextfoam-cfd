/*=========================================================================

   Program: ParaView
   Module:    pqServerResource.h

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef pqServerResource_h
#define pqServerResource_h

#include "pqCoreModule.h"

#include "vtkParaViewDeprecation.h" // for PARAVIEW_DEPRECATED_IN_5_11_0

#include <QString>
class pqServerConfiguration;

/**
 * pqServerResource encapsulates a resource in ParaView. A resource can be anything,
 * a data file, a list of data files, a state file, or a connection to a server.
 * The resource specification is based on URL-like syntax:
 * \verbatim
 * <scheme>://<scheme-specific-parameters>
 * \endverbatim
 *
 * To specify a data or state file, the following syntax is used:
 * \verbatim
 * <connection-scheme>:[//<server-details>]/<path-to-data-file>[#serverName]:KEY:1
 * \endverbatim
 *
 * \c connection-scheme can be
 * \li \c builtin - for builtin connections
 * \li \c cs - for client-server connections (pvserver)
 * \li \c csrc - for client-server connections with reverse-connect (pvserver rc)
 * \li \c cdsrs - for client-data-server-render-server connections (pvdataserver, pvrenderserver)
 * \li \c cdsrsrc - for cdsrs with reverse-connect.
 *
 * \c server-details are of the form \c \<serverhost-name\>:\<port\> or
 * \c \<dataserver-hostname\>:\<dataserver-port\>/\<renderserver-hostname\>:\<render-server-port\>
 * as applicable. Port numbers are always optional.
 *
 * The KEY should be PARAVIEW_DATA for data file and PARAVIEW_STATE for state file.
 *
 * Examples:
 * \verbatim
 * builtin:/home/user/foo.vtk
 * cs://amber1:11112/C:\Users\User\foo.vtk
 * cdsrsrc://amber2:11111/amber3:22222/home/user/foo.vtk#amberDRS
 * \endverbatim
 *
 * To specify a server-connection, without pointing to any data file(s), the
 * following syntax may be used:
 * \verbatim
 * <connection-scheme>:[//<server-details>]
 * \endverbatim
 *
 * Examples:
 * \verbatim
 * builtin:
 * cs://amber1:11112
 * cdsrsrc://amber2:11111/amber3:22222
 * \endverbatim
 *
 * As with data-files, port numbers are always optional in when specifying
 * server-details.
 *
 * Server name is also optional but will be used if available.
 *
 * Arbitrary data can be added to a resource. ParaView leverages this mechanism
 * to identify statefiles, to save additional files in a file series
 * when referring to a data file, or details about how to connect to the server
 * when referring to a server-connection.
 *
 * \sa pqServerResources, pqServer
 */
class PQCORE_EXPORT pqServerResource
{
public:
  pqServerResource();
  ~pqServerResource();
  pqServerResource& operator=(const pqServerResource&);

  /**
   * Create an pqServerResource from a provided URI
   */
  pqServerResource(const QString& uri);

  /**
   * Create an pqServerResource from a provided URI and store related configuration
   * If the configuration is named, the configuration name will be used as a serverName
   */
  pqServerResource(const QString& uri, const pqServerConfiguration& config);

  /**
   * Create a pqServerResource by copy
   */
  pqServerResource(const pqServerResource&);

  /**
   * Returns the pqServerConfiguration from which this resource was created, if any.
   */
  const pqServerConfiguration& configuration() const;

  /**
   * Returns a compact string representation of the complete resource in URI format, see above.
   */
  QString toURI() const;

  /**
   * Returns a compact string representation of the resource including extra data
   */
  QString serializeString() const;

  /**
   * Returns the resource scheme, builtin, cs, csrc, cdsrs or cdsrsrc
   */
  QString scheme() const;

  /**
   * Sets the resource scheme
   */
  void setScheme(const QString&);

  /**
   * Returns if the connection scheme is a reverse one
   */
  bool isReverse() const;

  /**
   * Returns the resource host, or empty string for builtin, cdsrs, and cdsrsrc schemes
   */
  QString host() const;

  /**
   * Sets the resource host
   */
  void setHost(const QString&);

  /**
   * Return the resource port, or -1 for builtin, cdsrs, and cdsrsrc schemes
   */
  int port() const;

  /**
   * Return the resource port, or default_port for builtin, cdsrs, and cdsrsrc schemes
   */
  int port(int default_port) const;

  /**
   * Set the resource port
   */
  void setPort(int);

  /**
   * Return the data server host, if any
   */
  QString dataServerHost() const;

  /**
   * Set the data server host
   */
  void setDataServerHost(const QString&);

  /**
   * Return the data server port, if any, or -1
   */
  int dataServerPort() const;

  /**
   * Return the data server port, if any, or default_port
   */
  int dataServerPort(int default_port) const;

  /**
   * Set the data server port
   */
  void setDataServerPort(int);

  /**
   * Return the render server host, if any
   */
  QString renderServerHost() const;

  /**
   * Set the render server host
   */
  void setRenderServerHost(const QString&);

  /**
   * Return the render server port, if any, or -1
   */
  int renderServerPort() const;

  /**
   * Return the render server port, if any, or default_port
   */
  int renderServerPort(int default_port) const;

  /**
   * Set the render server port
   */
  void setRenderServerPort(int);

  /**
   * Return the path to the resource, if any
   */
  QString path() const;

  /**
   * Set the path to the resource
   */
  void setPath(const QString&);

  /**
   * Return the server name of the resource, if any
   */
  QString serverName() const;

  /**
   * Set the server name of the resource
   */
  void setServerName(const QString& name);

  /**
   * Add a data on this resource, with a key/value logic
   */
  void addData(const QString& key, const QString& value);

  /**
   * Get data from this resource, associated with the provided key, if any
   */
  QString data(const QString& key) const;

  /**
   * Get data from this resource, associated with the provided key, if any. If not, return
   * default_value.
   */
  QString data(const QString& key, const QString& default_value) const;

  /**
   * Return true if this resource has data associated with the provided key, false otherwise.
   */
  bool hasData(const QString& key) const;

  /**
   * Returns a copy of this resource containing only server connection information -
   * scheme, host, and port numbers
   */
  pqServerResource schemeHostsPorts() const;

  /**
   * Returns a copy of this resource containing a subset of server information -
   * scheme and host (no port numbers)
   */
  pqServerResource schemeHosts() const;

  /**
   * Returns a copy of this resource containing only host and path information -
   * scheme and port numbers are excluded
   */
  pqServerResource hostPath() const;

  /**
   * Returns a copy of this resource containing only path and server name information -
   * scheme, host and port numbers are excluded
   */
  pqServerResource pathServerName() const;

  /**
   * Deprecated method, just return a copy of this
   */
  PARAVIEW_DEPRECATED_IN_5_11_0("Use the resource directly instead")
  pqServerResource sessionServer() const { return *this; };

  /**
   * Deprecated method, noop
   */
  PARAVIEW_DEPRECATED_IN_5_11_0("Use the resource directly instead")
  void setSessionServer(const pqServerResource&){};

  //@{
  /**
   * Operators comparing all elements of the resource
   */
  bool operator==(const pqServerResource&) const;
  bool operator!=(const pqServerResource&) const;
  bool operator<(const pqServerResource&) const;
  //@}

private:
  class pqImplementation;
  pqImplementation* const Implementation;
};

#endif
