<?xml version='1.0' encoding='UTF-8' ?>
<!--//
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
//-->
<xs:schema targetNamespace="http://www.w3.org/2005/Atom"
           elementFormDefault="qualified"
           attributeFormDefault="unqualified"
           xmlns:atom="http://www.w3.org/2005/Atom"
           xmlns:xs="http://www.w3.org/2001/XMLSchema">

  <xs:annotation>
    <xs:documentation>XML Schema for Validate the ATOM 1.0 Markup Language with "xhtmldbg"</xs:documentation>
    <xs:documentation>Maintainer: Juergen Heinemann (Undefined) http://www.hjcms.de</xs:documentation>
    <xs:documentation>Copyright: GPLv3</xs:documentation>
    <xs:documentation>MimeType: application/atom+xml</xs:documentation>
  </xs:annotation>

  <xs:import namespace="http://www.w3.org/XML/1998/namespace" schemaLocation="xml.xsd" />

  <xs:element name="feed" type="atom:feedType"/>
  <xs:element name="entry" type="atom:entryType"/>

  <xs:complexType name="textType" mixed="true">
    <xs:sequence>
      <xs:any namespace="http://www.w3.org/1999/xhtml" minOccurs="0"/>
    </xs:sequence>
    <xs:attribute name="type" >
      <xs:simpleType>
        <xs:restriction base="xs:token">
          <xs:enumeration value="text"/>
          <xs:enumeration value="html"/>
          <xs:enumeration value="xhtml"/>
        </xs:restriction>
      </xs:simpleType>
    </xs:attribute>
    <xs:attributeGroup ref="atom:commonAttributes"/>
  </xs:complexType>

  <xs:complexType name="personType">
    <xs:choice minOccurs="1" maxOccurs="unbounded">
      <xs:element name="name" type="xs:string" minOccurs="1" maxOccurs="1" />
      <xs:element name="uri" type="atom:uriType" minOccurs="0" maxOccurs="1" />
      <xs:element name="email" type="atom:emailType" minOccurs="0" maxOccurs="1" />
      <xs:any namespace="##other"/>
    </xs:choice>
    <xs:attributeGroup ref="atom:commonAttributes"/>
  </xs:complexType>

  <xs:simpleType name="emailType">
    <xs:restriction base="xs:normalizedString">
      <xs:pattern value="([a-zA-Z0-9_\-])([a-zA-Z0-9_\-\.]*)@(\[((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}|((([a-zA-Z0-9\-]+)\.)+))([a-zA-Z]{2,}|(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\])" />
    </xs:restriction>
  </xs:simpleType>

  <xs:complexType name="feedType">
    <xs:choice minOccurs="3" maxOccurs="unbounded">
      <xs:element name="author" type="atom:personType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="category" type="atom:categoryType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="contributor" type="atom:personType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="generator" type="atom:generatorType" minOccurs="0" maxOccurs="1" />
      <xs:element name="icon" type="atom:iconType" minOccurs="0" maxOccurs="1" />
      <xs:element name="id" type="atom:idType" minOccurs="1" maxOccurs="1" />
      <xs:element name="link" type="atom:linkType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="logo" type="atom:logoType" minOccurs="0" maxOccurs="1" />
      <xs:element name="rights" type="atom:textType" minOccurs="0" maxOccurs="1" />
      <xs:element name="subtitle" type="atom:textType" minOccurs="0" maxOccurs="1" />
      <xs:element name="title" type="atom:textType" minOccurs="1" maxOccurs="1" />
      <xs:element name="updated" type="atom:dateTimeType" minOccurs="1" maxOccurs="1" />
      <xs:element name="entry" type="atom:entryType" minOccurs="0" maxOccurs="unbounded" />
      <xs:any namespace="##other" minOccurs="0" maxOccurs="unbounded"/>
    </xs:choice>
    <xs:attributeGroup ref="atom:commonAttributes"/>
  </xs:complexType>

  <xs:complexType name="entryType">
    <xs:choice maxOccurs="unbounded">
      <xs:element name="author" type="atom:personType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="category" type="atom:categoryType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="content" type="atom:contentType" minOccurs="0" maxOccurs="1" />
      <xs:element name="contributor" type="atom:personType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="id" type="atom:idType" minOccurs="1" maxOccurs="1" />
      <xs:element name="link" type="atom:linkType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="published" type="atom:dateTimeType" minOccurs="0" maxOccurs="1" />
      <xs:element name="rights" type="atom:textType" minOccurs="0" maxOccurs="1" />
      <xs:element name="source" type="atom:textType" minOccurs="0" maxOccurs="1" />
      <xs:element name="summary" type="atom:textType" minOccurs="0" maxOccurs="1" />
      <xs:element name="title" type="atom:textType" minOccurs="1" maxOccurs="1" />
      <xs:element name="updated" type="atom:dateTimeType" minOccurs="1" maxOccurs="1" />
      <xs:any namespace="##other" minOccurs="0" maxOccurs="unbounded"/>
    </xs:choice>
    <xs:attributeGroup ref="atom:commonAttributes"/>
  </xs:complexType>

  <xs:complexType name="contentType" mixed="true">
    <xs:sequence>
      <xs:any namespace="##other" minOccurs="0" maxOccurs="unbounded" />
    </xs:sequence>
    <xs:attribute name="type" type="xs:string"/>
    <xs:attribute name="src" type="xs:anyURI"/>
    <xs:attributeGroup ref="atom:commonAttributes"/>
  </xs:complexType>

  <xs:complexType name="categoryType">
    <xs:attribute name="term" type="xs:string" use="required"/>
    <xs:attribute name="scheme" type="xs:anyURI" use="optional"/>
    <xs:attribute name="label" type="xs:string" use="optional"/>
    <xs:attributeGroup ref="atom:commonAttributes" />
  </xs:complexType>

  <xs:complexType name="generatorType">
    <xs:simpleContent>
      <xs:extension base="xs:string">
        <xs:attribute name="uri" use="optional" type="xs:anyURI" />
        <xs:attribute name="version" use="optional" type="xs:string" />
        <xs:attributeGroup ref="atom:commonAttributes"/>
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="iconType">
    <xs:simpleContent>
      <xs:extension base="xs:anyURI">
        <xs:attributeGroup ref="atom:commonAttributes"/>
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="idType">
    <xs:simpleContent>
      <xs:extension base="xs:anyURI">
        <xs:attributeGroup ref="atom:commonAttributes"/>
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="linkType" mixed="true">
    <xs:attribute name="href" use="required" type="xs:anyURI" />
    <xs:attribute name="rel" type="xs:string" use="optional"/>
    <xs:attribute name="type" use="optional" type="xs:string" />
    <xs:attribute name="hreflang" use="optional" type="xs:NMTOKEN" />
    <xs:attribute name="title" use="optional" type="xs:string" />
    <xs:attribute name="length" use="optional" type="xs:positiveInteger" />
    <xs:attributeGroup ref="atom:commonAttributes"/>
  </xs:complexType>

  <xs:complexType name="logoType">
    <xs:simpleContent>
      <xs:extension base="xs:anyURI">
        <xs:attributeGroup ref="atom:commonAttributes"/>
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="sourceType">
    <xs:choice maxOccurs="unbounded">
      <xs:element name="author" type="atom:personType" minOccurs="0" maxOccurs="unbounded"/>
      <xs:element name="category" type="atom:categoryType" minOccurs="0" maxOccurs="unbounded"/>
      <xs:element name="contributor" type="atom:personType" minOccurs="0" maxOccurs="unbounded"/>
      <xs:element name="generator" type="atom:generatorType" minOccurs="0" maxOccurs="1"/>
      <xs:element name="icon" type="atom:iconType" minOccurs="0" maxOccurs="1"/>
      <xs:element name="id" type="atom:idType" minOccurs="0" maxOccurs="1"/>
      <xs:element name="link" type="atom:linkType" minOccurs="0" maxOccurs="unbounded"/>
      <xs:element name="logo" type="atom:logoType" minOccurs="0" maxOccurs="1"/>
      <xs:element name="rights" type="atom:textType" minOccurs="0" maxOccurs="1"/>
      <xs:element name="subtitle" type="atom:textType" minOccurs="0" maxOccurs="1"/>
      <xs:element name="title" type="atom:textType" minOccurs="0" maxOccurs="1"/>
      <xs:element name="updated" type="atom:dateTimeType" minOccurs="0" maxOccurs="1"/>
      <xs:any namespace="##other" minOccurs="0" maxOccurs="unbounded"/>
    </xs:choice>
    <xs:attributeGroup ref="atom:commonAttributes"/>
  </xs:complexType>

  <xs:complexType name="uriType">
    <xs:simpleContent>
      <xs:extension base="xs:anyURI">
        <xs:attributeGroup ref="atom:commonAttributes"/>
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="dateTimeType">
    <xs:simpleContent>
      <xs:extension base="xs:dateTime">
        <xs:attributeGroup ref="atom:commonAttributes"/>
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:attributeGroup name="commonAttributes">
    <xs:attribute ref="xml:base" />
    <xs:attribute ref="xml:lang" />
    <xs:anyAttribute namespace="##other"/>
  </xs:attributeGroup>

</xs:schema>