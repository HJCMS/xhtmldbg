<?xml version='1.0' encoding='UTF-8' ?>
<!--//
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
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
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
           elementFormDefault="unqualified"
           attributeFormDefault="unqualified"
           xml:lang="en" version="0.8.8" id="rss-2.0">

  <xs:annotation>
    <xs:documentation>XML Schema for Validate the RSS 2.0 Markup Language with "xhtmldbg"</xs:documentation>
    <xs:documentation>Maintainer: Juergen Heinemann (Undefined) http://www.hjcms.de</xs:documentation>
    <xs:documentation>Copyright: GPLv3</xs:documentation>
    <xs:documentation>MimeType: application/rss+xml</xs:documentation>
  </xs:annotation>

  <!-- xs:NCName -->
  <xs:import namespace="http://www.w3.org/2005/Atom" schemaLocation="atom-1.0.xsd" />
  <xs:import namespace="http://www.w3.org/XML/1998/namespace" schemaLocation="xml.xsd" />

  <xs:element name="rss">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="channel" type="channelNode" minOccurs="1" maxOccurs="1" />
        <xs:any namespace="http://www.w3.org/2005/Atom" processContents="lax"
                minOccurs="0" maxOccurs="unbounded" />
      </xs:sequence>
      <xs:attribute name="version" type="xs:decimal" use="required" fixed="2.0" />
      <xs:anyAttribute namespace="##other"/>
    </xs:complexType>
  </xs:element>

  <xs:complexType name="channelNode">
    <xs:sequence>
      <xs:choice maxOccurs="unbounded">
        <!-- Required -->
        <xs:element name="title" type="xs:string" minOccurs="1" />
        <xs:element name="link" type="xs:anyURI" minOccurs="1" />
        <xs:element name="description" type="xs:string" minOccurs="1" />
        <!-- Optional -->
        <xs:element name="language" type="xs:language" minOccurs="0" />
        <xs:element name="copyright" type="xs:string" minOccurs="0" />
        <xs:element name="managingEditor" type="xs:string" minOccurs="0" />
        <xs:element name="webMaster" type="personResponsibleNode" minOccurs="0" />
        <xs:element name="pubDate" type="dateNode" minOccurs="0" />
        <xs:element name="lastBuildDate" type="dateNode" minOccurs="0" />
        <xs:element name="category" type="categoryNode" minOccurs="0" />
        <xs:element name="generator" type="xs:string" minOccurs="0" />
        <xs:element name="docs" type="xs:string" minOccurs="0" />
        <xs:element name="cloud" type="cloudNode" minOccurs="0" />
        <xs:element name="ttl" type="xs:positiveInteger" minOccurs="0" />
        <xs:element name="image" type="imageNode" minOccurs="0" />
        <xs:element name="rating" type="xs:string" minOccurs="0" />
        <xs:element name="textInput" type="textInputNode" minOccurs="0" />
        <xs:element name="skipHours" type="skipHoursNode" minOccurs="0" />
        <xs:element name="skipDays" type="skipDaysNode" minOccurs="0" />
        <xs:element name="item" type="itemNodes" minOccurs="1" />
        <!-- ATOM -->
        <xs:any namespace="http://www.w3.org/2005/Atom"
                minOccurs="0" maxOccurs="unbounded"
                processContents="lax" />
      </xs:choice>
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="itemNodes">
    <xs:choice maxOccurs="unbounded">
      <!-- Required -->
      <xs:element name="title" type="xs:string" minOccurs="1" />
      <xs:element name="link" type="xs:anyURI" minOccurs="1" />
      <xs:element name="description" type="descriptionNode" minOccurs="1" />
      <!-- Optional -->
      <xs:element name="author" type="personResponsibleNode" minOccurs="0" />
      <xs:element name="category" type="categoryNode" minOccurs="0" />
      <xs:element name="comments" type="xs:string" minOccurs="0" />
      <xs:element name="enclosure" type="enclosureNode" minOccurs="0" />
      <xs:element name="guid" type="guidNode" minOccurs="0" />
      <xs:element name="pubDate" type="dateNode" minOccurs="0" />
      <xs:element name="source" type="sourceNode" minOccurs="0" />
    </xs:choice>
  </xs:complexType>

  <xs:simpleType name="dateNode">
    <xs:restriction base="xs:string">
      <xs:pattern value="(((Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)), *)?\d\d? +((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +\d\d(\d\d)? +\d\d:\d\d(:\d\d)? +(([+\-]?\d\d\d\d)|(UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|\w)" />
    </xs:restriction>
  </xs:simpleType>

  <xs:simpleType name="personResponsibleNode">
    <xs:restriction base="xs:normalizedString">
      <xs:pattern value="([a-zA-Z0-9_\-])([a-zA-Z0-9_\-\.]*)@(\[((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}|((([a-zA-Z0-9\-]+)\.)+))([a-zA-Z]{2,}|(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\])"/>
    </xs:restriction>
  </xs:simpleType>

  <xs:complexType name="sourceNode">
    <xs:simpleContent>
      <xs:extension base="xs:string">
        <xs:attribute name="url" type="xs:anyURI"/>
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="enclosureNode">
    <xs:simpleContent>
      <xs:extension base="xs:string">
        <xs:attribute name="url" type="xs:anyURI" use="required" />
        <xs:attribute name="length" type="xs:byte" use="required" />
        <xs:attribute name="type" type="xs:string" use="required" />
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="categoryNode">
    <xs:simpleContent>
      <xs:extension base="xs:string">
        <xs:attribute name="domain" type="xs:anyURI" use="optional" />
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:complexType name="cloudNode" block="restriction">
    <xs:attribute name="domain" type="xs:anyURI" use="required" />
    <xs:attribute name="port" type="xs:positiveInteger" use="required" />
    <xs:attribute name="path" type="xs:anyURI" use="required" />
    <xs:attribute name="registerProcedure" type="xs:anyURI" use="required" />
    <xs:attribute name="protocol" type="cloudAttributeProtocol" use="required" />
  </xs:complexType>

  <xs:complexType name="textInputNode" block="restriction">
    <xs:all>
      <xs:element name="title" type="xs:string" minOccurs="1" />
      <xs:element name="description" type="descriptionNode" minOccurs="1" />
      <xs:element name="link" type="xs:anyURI" minOccurs="1" />
      <xs:element name="name" type="xs:string" minOccurs="1" />
    </xs:all>
  </xs:complexType>

  <xs:complexType name="skipHoursNode" block="restriction">
    <xs:sequence>
      <xs:element name="hour" minOccurs="0" maxOccurs="24">
        <xs:simpleType>
          <xs:restriction base="xs:nonNegativeInteger">
            <xs:minInclusive value="0"/>
            <xs:maxInclusive value="23"/>
          </xs:restriction>
        </xs:simpleType>
      </xs:element>
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="skipDaysNode" block="restriction">
    <xs:sequence>
      <xs:element name="day" minOccurs="1" maxOccurs="7">
        <xs:simpleType>
          <xs:restriction base="xs:string">
            <xs:enumeration value="Monday"/>
            <xs:enumeration value="Tuesday"/>
            <xs:enumeration value="Wednesday"/>
            <xs:enumeration value="Thursday"/>
            <xs:enumeration value="Friday"/>
            <xs:enumeration value="Saturday"/>
            <xs:enumeration value="Sunday"/>
          </xs:restriction>
        </xs:simpleType>
      </xs:element>
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="imageNode" block="restriction">
    <xs:all>
      <xs:element name="url" minOccurs="1">
        <xs:simpleType>
          <xs:restriction base="xs:string">
            <xs:pattern value="(http:.+\.(gif|GIF|jp[e]?g|JP[E]?G|png|PNG))"/>
          </xs:restriction>
        </xs:simpleType>
      </xs:element>
      <xs:element name="title" type="xs:string" minOccurs="1" />
      <xs:element name="link" type="xs:anyURI" minOccurs="1" />
    </xs:all>
  </xs:complexType>

  <xs:complexType name="descriptionNode">
     <xs:complexContent mixed="true">
      <xs:restriction base="xs:anyType">
        <xs:sequence>
          <xs:any namespace="http://www.w3.org/1999/xhtml"
                  minOccurs="0" maxOccurs="unbounded"
                  processContents="skip" />
        </xs:sequence>
        <xs:anyAttribute namespace="http://www.w3.org/1999/xhtml" />
      </xs:restriction>
    </xs:complexContent>
  </xs:complexType>

  <xs:complexType name="guidNode">
    <xs:simpleContent>
      <xs:extension base="xs:anyURI">
        <xs:attribute name="isPermaLink" type="xs:boolean" use="optional" />
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>

  <xs:simpleType name="cloudAttributeProtocol">
    <xs:restriction base="xs:string">
      <xs:enumeration value="xml-rpc"/>
      <xs:enumeration value="http-post"/>
      <xs:enumeration value="soap"/>
    </xs:restriction>
  </xs:simpleType>

</xs:schema>
