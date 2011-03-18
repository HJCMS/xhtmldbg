<?xml version='1.0' encoding='UTF-8' ?>
<!DOCTYPE xs:schema PUBLIC "-//W3C//DTD XMLSCHEMA 200102//EN" "XMLSchema.dtd" [
  <!ENTITY % xs-metalink PUBLIC 'metalink' 'metalink-3.0.dtd' >
]>
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
<xs:schema version="3.0" xmlns="http://www.metalinker.org/"
           xmlns:xs="http://www.w3.org/2001/XMLSchema"
           targetNamespace="http://www.metalinker.org/"
           elementFormDefault="qualified" 
           attributeFormDefault="unqualified">

  <xs:annotation>
    <xs:documentation>XML Schema for Validate the Metalink Markup Language with "xhtmldbg"</xs:documentation>
    <xs:documentation>Maintainer: Juergen Heinemann (Undefined) http://www.hjcms.de</xs:documentation>
    <xs:documentation>Copyright: GPLv3</xs:documentation>
    <xs:documentation>MimeType: application/metalink4+xml</xs:documentation>
  </xs:annotation>

  <!-- xs:NCName -->
  <xs:import namespace="http://www.w3.org/XML/1998/namespace" schemaLocation="xml.xsd" />

  <xs:element name="metalink" type="metalinkType" />
  <xs:complexType name="filesType">
    <xs:sequence>
      <xs:element name="file" type="fileType" maxOccurs="unbounded" />
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="metalinkType">
    <xs:all>
      <xs:element minOccurs="0" name="description" type="xs:string" />
      <xs:element minOccurs="0" name="identity" type="xs:string" />
      <xs:element minOccurs="0" name="license" type="licenseType" />
      <xs:element minOccurs="0" name="publisher" type="publisherType" />
      <xs:element minOccurs="0" name="releasedate" type="RFC822DateTime" />
      <xs:element minOccurs="0" name="tags" type="xs:string" />
      <xs:element minOccurs="0" name="version" type="xs:string" />
      <xs:element name="files" type="filesType" />
    </xs:all>
    <xs:attribute name="version" type="xs:string" default="3.0" />
    <xs:attribute name="origin" type="xs:anyURI" />
    <xs:attribute name="type" default="static">
      <xs:simpleType>
        <xs:restriction base="xs:string">
          <xs:enumeration value="dynamic" />
          <xs:enumeration value="static" />
        </xs:restriction>
      </xs:simpleType>
    </xs:attribute>
    <xs:attribute name="pubdate" type="RFC822DateTime" />
    <xs:attribute name="refreshdate" type="RFC822DateTime" />
    <xs:attribute name="generator" type="xs:string" />
  </xs:complexType>

  <xs:complexType name="fileType">
    <xs:all>
      <xs:element minOccurs="0" name="changelog" type="xs:string" />
      <xs:element minOccurs="0" name="copyright" type="xs:string" />
      <xs:element minOccurs="0" name="description" type="xs:string" />
      <xs:element minOccurs="0" name="identity" type="xs:string" />
      <xs:element minOccurs="0" name="language" type="xs:language" />
      <xs:element minOccurs="0" name="license" type="licenseType" />
      <xs:element minOccurs="0" name="logo" type="xs:string" />
      <xs:element minOccurs="0" name="mimetype" type="xs:string" />
      <xs:element minOccurs="0" name="multimedia" type="multimediaType" />
      <xs:element minOccurs="0" name="os" type="xs:string" />
      <xs:element minOccurs="0" name="publisher" type="publisherType" />
      <xs:element minOccurs="0" name="relations" type="xs:string" />
      <xs:element minOccurs="0" name="releasedate" type="RFC822DateTime" />
      <xs:element name="resources" type="resourcesType" />
      <xs:element minOccurs="0" name="size" type="xs:nonNegativeInteger" />
      <xs:element minOccurs="0" name="screenshot" type="xs:string" />
      <xs:element minOccurs="0" name="tags" type="xs:string" />
      <xs:element minOccurs="0" name="upgrade" type="xs:string" />
      <xs:element minOccurs="0" name="verification">
        <xs:complexType>
          <xs:sequence>
            <xs:element name="hash" minOccurs="0" maxOccurs="unbounded">
              <xs:complexType>
                <xs:simpleContent>
                  <xs:extension base="xs:string">
                    <xs:attribute name="type" type="hashType" default="sha1" />
                  </xs:extension>
                </xs:simpleContent>
              </xs:complexType>
            </xs:element>
            <xs:element name="signature" minOccurs="0" maxOccurs="unbounded">
              <xs:complexType>
                <xs:simpleContent>
                  <xs:extension base="xs:string">
                    <xs:attribute name="type" default="PGP" type="xs:string" />
                    <xs:attribute name="file" type="xs:string" />
                  </xs:extension>
                </xs:simpleContent>
              </xs:complexType>
            </xs:element>
            <xs:element name="pieces" minOccurs="0">
              <xs:complexType>
                <xs:sequence>
                  <xs:element name="hash" maxOccurs="unbounded">
                    <xs:complexType>
                      <xs:simpleContent>
                        <xs:extension base="xs:string">
                          <xs:attribute name="piece">
                            <xs:simpleType>
                              <xs:restriction base="xs:integer">
                                <xs:minInclusive value="0" />
                              </xs:restriction>
                            </xs:simpleType>
                          </xs:attribute>
                        </xs:extension>
                      </xs:simpleContent>
                    </xs:complexType>
                  </xs:element>
                </xs:sequence>
                <xs:attribute name="length" type="xs:nonNegativeInteger" />
                <xs:attribute name="type" type="hashType" />
              </xs:complexType>
            </xs:element>
          </xs:sequence>
        </xs:complexType>
      </xs:element>
      <xs:element minOccurs="0" name="version" type="xs:string" />
    </xs:all>
    <xs:attribute name="name" type="xs:string" />
  </xs:complexType>

  <xs:complexType name="resourcesType">
    <xs:sequence>
      <xs:element name="url" maxOccurs="unbounded">
        <xs:complexType>
          <xs:simpleContent>
            <xs:extension base="xs:string">
              <xs:attribute name="type" default="http">
                <xs:simpleType>
                  <xs:restriction base="xs:string">
                      <xs:enumeration value="ftp" />
                      <xs:enumeration value="ftps" />
                      <xs:enumeration value="http" />
                      <xs:enumeration value="https" />
                      <xs:enumeration value="rsync" />
                      <xs:enumeration value="bittorrent" />
                      <xs:enumeration value="magnet" />
                      <xs:enumeration value="ed2k" />
                      <xs:enumeration value="gnunet" />
                  </xs:restriction>
                </xs:simpleType>
              </xs:attribute>
              <xs:attribute name="location">
                <xs:simpleType>
                  <xs:restriction base="xs:string">
                      <xs:maxLength value="2" />
                  </xs:restriction>
                </xs:simpleType>
              </xs:attribute>
              <xs:attribute name="preference" type="xs:nonNegativeInteger" />
              <xs:attribute name="maxconnections" type="xs:positiveInteger" />
            </xs:extension>
          </xs:simpleContent>
        </xs:complexType>
      </xs:element>
    </xs:sequence>
    <xs:attribute name="maxconnections" type="xs:positiveInteger" />
  </xs:complexType>

  <xs:complexType name="licenseType">
    <xs:sequence>
      <xs:element name="name" type="xs:string" />
      <xs:element name="url" type="xs:string" minOccurs="0" />
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="publisherType">
    <xs:sequence>
      <xs:element name="name" type="xs:string" />
      <xs:element name="url" type="xs:string" minOccurs="0" />
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="audioType">
    <xs:sequence>
      <xs:element minOccurs="0" name="bitrate" type="xs:string" />
      <xs:element minOccurs="0" name="codec" type="xs:string" />
      <xs:element minOccurs="0" name="duration" type="xs:string" />
      <xs:element minOccurs="0" name="resolution" type="xs:string" />
      <xs:element minOccurs="0" name="artist" type="xs:string" />
      <xs:element minOccurs="0" name="album" type="xs:string" />
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="videoType">
    <xs:sequence>
      <xs:element minOccurs="0" name="bitrate" type="xs:string" />
      <xs:element minOccurs="0" name="codec" type="xs:string" />
      <xs:element minOccurs="0" name="duration" type="xs:string" />
      <xs:element minOccurs="0" name="resolution" type="xs:string" />
      <xs:element minOccurs="0" name="artist" type="xs:string" />
      <xs:element minOccurs="0" name="album" type="xs:string" />
    </xs:sequence>
  </xs:complexType>

  <xs:complexType name="multimediaType">
    <xs:sequence>
      <xs:element name="audio" type="audioType" minOccurs="0" maxOccurs="unbounded" />
      <xs:element name="video" type="videoType" minOccurs="0" maxOccurs="unbounded" />
    </xs:sequence>
  </xs:complexType>

  <xs:simpleType name="RFC822DateTime">
    <xs:restriction base="xs:string">
      <xs:pattern value="(((Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)|(Sun)), *)?\d\d? +((Jan)|(Feb)|(Mar)|(Apr)|(May)|(Jun)|(Jul)|(Aug)|(Sep)|(Oct)|(Nov)|(Dec)) +\d\d(\d\d)? +\d\d:\d\d(:\d\d)? +(([+\-]?\d\d\d\d)|(UT)|(GMT)|(EST)|(EDT)|(CST)|(CDT)|(MST)|(MDT)|(PST)|(PDT)|\w)" />
    </xs:restriction>
  </xs:simpleType>

  <xs:simpleType name="hashType">
    <xs:restriction base="xs:string">
      <xs:enumeration value="ed2k" />
      <xs:enumeration value="md4" />
      <xs:enumeration value="md5" />
      <xs:enumeration value="sha1" />
      <xs:enumeration value="sha256" />
      <xs:enumeration value="sha384" />
      <xs:enumeration value="sha512" />
      <xs:enumeration value="ripemd160" />
      <xs:enumeration value="rmd160" />
      <xs:enumeration value="tiger" />
      <xs:enumeration value="tiger192" />
      <xs:enumeration value="crc32" />
    </xs:restriction>
  </xs:simpleType>

</xs:schema>
