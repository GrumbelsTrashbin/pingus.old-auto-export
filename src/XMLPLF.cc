//  $Id: XMLPLF.cc,v 1.9 2000/09/13 00:14:56 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <cstdio>
#include <cstdlib>
#include "XMLPLF.hh"
#include "XMLhelper.hh"
#include "globals.hh"
#include "System.hh"
#include "PingusError.hh"
#include "StringConverter.hh"

XMLPLF::XMLPLF()
{
}

XMLPLF::XMLPLF(const string& filename)
{
  //  std::cout << "----- Parsing .xml file" << std::endl;

  doc = xmlParseFile(filename.c_str());

  if (doc == NULL)
    throw PingusError("Couldn't open \"" + filename + "\"");

  parse_file();
}

XMLPLF::~XMLPLF()
{
  xmlFreeDoc(doc);
}

void
XMLPLF::parse_file()
{
  xmlNodePtr cur = doc->root;

  if (cur != NULL && strcmp((const char*)cur->name, "pingus-level") == 0)
    {
      cur = cur->childs;
      while (cur != NULL)
	{
	  //puts("global loop");
	  if (strcmp((char*)cur->name, "global") == 0)
	    {
	      parse_global(cur);
	    }
	  else if (strcmp((char*)cur->name, "action-list") == 0)
	    {
	      parse_actions(cur);
	    }
	  else if (strcmp((char*)cur->name, "background") == 0)
	    {
	      parse_background(cur);
	    }
	  else if (strcmp((char*)cur->name, "groundpiece") == 0)
	    {
	      parse_groundpiece(cur);
	    }
	  else if (strcmp((char*)cur->name, "exit") == 0)
	    {
	      parse_exit(cur);
	    }
	  else if (strcmp((char*)cur->name, "entrance") == 0)
	    {
	      parse_entrance(cur);
	    }
	  else if (strcmp((char*)cur->name, "trap") == 0)
	    {
	      parse_traps(cur);
	    }
	  else if (strcmp((char*)cur->name, "hotspot") == 0)
	    {
	      parse_hotspot(cur);
	    }
	  else if (strcmp((char*)cur->name, "liquid") == 0)
	    {
	      parse_liquid(cur);
	    }
	  else if (strcmp((char*)cur->name, "group") == 0)
	    {
	      parse_group(cur);
	    }
	  else if (strcmp ((char*)cur->name, "start-position") == 0)
	    {
	      parse_start_pos(cur);
	    }
	  else if (strcmp ((char*)cur->name, "weather") == 0)
	    {
	      parse_weather(cur);
	    }	  
	  else
	    {
	      printf("Unhandled: %s\n", (char*)cur->name);
	    }
	  cur = cur->next;
	}
    } else {
      throw PingusError("XMLPLF: This is no valid Pingus level");
    }
}

void
XMLPLF::parse_start_pos(xmlNodePtr cur)
{
  cur = cur->childs;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "position") == 0)
	{
	  Position pos = XMLhelper::parse_position(doc, cur);
	  start_x_pos = pos.x_pos;
	  start_y_pos = pos.y_pos;
	}
      else
	{
	  std::cout << "XMLPLF::parse_start_pos: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
}

void
XMLPLF::parse_weather(xmlNodePtr cur)
{
  WeatherData weather;
  cur = cur->childs;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "type") == 0)
	{
	  weather.type = XMLhelper::parse_string(doc, cur);
	}
      else
	{
	  std::cout << "XMLPLF: Unhandeld: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
  weathers.push_back(weather);
}

void
XMLPLF::parse_group(xmlNodePtr cur)
{
  cur = cur->childs;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "groundpiece") == 0)
	{
	  parse_groundpiece(cur);
	}
      else if (strcmp((char*)cur->name, "exit") == 0)
	{
	  parse_exit(cur);
	}
      else if (strcmp((char*)cur->name, "entrance") == 0)
	{
	  parse_entrance(cur);
	}
      else if (strcmp((char*)cur->name, "trap") == 0)
	{
	  parse_traps(cur);
	}
      else if (strcmp((char*)cur->name, "hotspot") == 0)
	{
	  parse_hotspot(cur);
	}
      else if (strcmp((char*)cur->name, "liquid") == 0)
	{
	  parse_liquid(cur);
	}
      else if (strcmp((char*)cur->name, "group") == 0)
	{
	  parse_group(cur);
	}
      else
	{
	  printf("Unhandled: %s\n", (char*)cur->name);
	}
      cur = cur->next;
    } 
}

void
XMLPLF::parse_liquid(xmlNodePtr cur)
{
  LiquidData liquid;

  cur = cur->childs;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "position") == 0)
	liquid.pos = XMLhelper::parse_position(doc, cur);
      else if (strcmp((char*)cur->name, "surface") == 0)
	liquid.desc = XMLhelper::parse_surface(doc, cur);
      else if (strcmp((char*)cur->name, "speed") == 0)
	liquid.speed = XMLhelper::parse_int(doc, cur);
      else if (strcmp((char*)cur->name, "width") == 0)
	liquid.width = XMLhelper::parse_int(doc, cur);
      else
	{
	  std::cout << "XMLPLF::parse_liquid: Unhandled: " << cur->name << std::endl;
	}	

      cur = cur->next;
    }
  liquids.push_back(liquid);
}

void 
XMLPLF::parse_background(xmlNodePtr cur)
{
  // FIXME: Memory leak?!
  backgrounds.push_back(BackgroundData::create (doc, cur));
}

void 
XMLPLF::parse_actions(xmlNodePtr cur)
{
  cur = cur->childs;
  while (cur != NULL)
    {
      char* number = (char*)xmlNodeListGetString(doc, cur->childs, 1);

      ActionData button((char*)cur->name, StringConverter::to_int(number));

      actions.push_back(button);

      free(number);
      cur = cur->next;
    }      
}

void 
XMLPLF::parse_entrance(xmlNodePtr cur)
{
  EntranceData entrance;
  cur = cur->childs;  
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "type") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->childs, 1); 
	  entrance.type = name;
	  free(name);
	}
      else if (strcmp((char*)cur->name, "position") == 0)
	{
	  entrance.pos = XMLhelper::parse_position(doc, cur);
	}
      else if (strcmp((char*)cur->name, "release-rate") == 0)
	{
	  char* release_rate = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  entrance.release_rate = StringConverter::to_int(release_rate);
	  free(release_rate);
	}
      else if (strcmp((char*)cur->name, "direction") == 0)
	{
	  char* direction = (char*)xmlNodeListGetString(doc, cur->childs, 1);

	  if (strcmp(direction, "left") == 0)
	    entrance.direction = EntranceData::LEFT;
	  else if (strcmp(direction, "right") == 0)
	    entrance.direction = EntranceData::RIGHT;
	  else if (strcmp(direction, "misc") == 0)
	    entrance.direction = EntranceData::MISC;
	  
	  free(direction);
	}
      else
	{
	  printf("unhandeld: %s\n", (char*)cur->name);
	}	
      cur = cur->next;	
    }   
  entrances.push_back(entrance);
}

void 
XMLPLF::parse_exit(xmlNodePtr cur)
{
  ExitData exit;
  cur = cur->childs;
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "position") == 0)
	{
	  exit.pos = XMLhelper::parse_position(doc, cur);
	}
      else if (strcmp((char*)cur->name, "surface") == 0)
	{
	  exit.desc = XMLhelper::parse_surface(doc, cur);
	}
      cur = cur->next;	
    }

  exits.push_back(exit);
}

void
XMLPLF::parse_global(xmlNodePtr cur)
{
  cur = cur->childs;
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "levelname") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  char* lang = (char*)xmlGetProp(cur, (xmlChar*)"lang");

	  if (name) {
	    if (lang)
	      levelname[lang] = name;
	    else
	      levelname[default_language] = name;
	  }

	  if (name) free(name);
	  if (lang) free(lang);
	}
      else if (strcmp((char*)cur->name, "description") == 0)
	{
	  char* desc = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  char* lang = (char*)xmlGetProp(cur, (xmlChar*)"lang");

	  if (desc) {
	    if (lang)		    
	      description[lang] = desc;
	    else
	      description[default_language] = desc;
	  }

	  if (desc) free(desc);	  
	  if (lang) free(lang);
	}
      else if (strcmp((char*)cur->name, "author") == 0)
	{
	  char* tmp_author = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  if (tmp_author) {
	    author = tmp_author;
	    free(tmp_author);
	  }
	  //std::cout << "Author: " << author << " -----------------------" << std::endl;
	}
      else if (strcmp((char*)cur->name, "number-of-pingus") == 0)
	{
	  number_of_pingus = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "number-to-save") == 0)
	{
	  number_to_save = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "time") == 0)
	{
	  max_time = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "height") == 0)
	{
	  height = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "width") == 0)
	{
	  width = XMLhelper::parse_int(doc, cur);
	}
      else
	{
	  std::cout << "XMLPLF: global: unhandeld: " << cur->name << std::endl;
	}

      cur = cur->next;
    }
}

void 
XMLPLF::parse_groundpiece(xmlNodePtr cur)
{
  SurfaceData surface;

  surface.type = SurfaceData::GROUND;

  char* type = (char*)xmlGetProp(cur, (xmlChar*)"type");
  if (type)
    {
      // std::cout << "groundpiece type: " << type << std::endl;

      if (strcmp(type, "solid") == 0) 
	surface.type = SurfaceData::SOLID;
      else if (strcmp(type, "transparent") == 0) 
	surface.type = SurfaceData::TRANSPARENT;
      else if (strcmp(type, "ground") == 0) 
	surface.type = SurfaceData::GROUND;
      else if (strcmp(type, "bridge") == 0) 
	surface.type = SurfaceData::BRIDGE;
      else
	std::cout << "XMLPLF: Unhandeld groundpiece type: " << type << std::endl;
      free(type);
    }
  else
    std::cout << "XMLPLF: groundtype empty" << std::endl;

  cur = cur->childs;

  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "position") == 0)
	{
	  surface.pos = XMLhelper::parse_position(doc, cur);
	}
      else if (strcmp((char*)cur->name, "surface") == 0)
	{
	  surface.desc = XMLhelper::parse_surface(doc, cur);
	}	
      else
	{
	  printf("unhandled: %s\n", (char*)cur->name);
	}
      cur = cur->next;	
    }
  groundpieces.push_back(surface);
}

void
XMLPLF::parse_traps(xmlNodePtr cur)
{
  TrapData trap;
  cur = cur->childs;
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "type") == 0)
	{
	  char* name = (char*)xmlNodeListGetString(doc, cur->childs, 1);
	  if (name)
	    {
	      // std::cout << "parse_trap: name = " << name << std::endl;
	      trap.type = name;
	      free(name);
	    }
	} 
      else if (strcmp((char*)cur->name, "position") == 0) 
	{
	  trap.pos = XMLhelper::parse_position(doc, cur);
	}

      cur = cur->next;
    }
  traps.push_back(trap);
}  

void
XMLPLF::parse_hotspot(xmlNodePtr cur)
{
  HotspotData hotspot;
  cur = cur->childs;
  while (cur != NULL)
    {
      if (strcmp((char*)cur->name, "surface") == 0)
	{
	  hotspot.desc = XMLhelper::parse_surface(doc, cur);
	} 
      else if (strcmp((char*)cur->name, "position") == 0) 
	{
	  hotspot.pos = XMLhelper::parse_position(doc, cur);
	}
      else if (strcmp((char*)cur->name, "speed") == 0) 
	{
	  hotspot.speed = XMLhelper::parse_int(doc, cur);
	}
      else if (strcmp((char*)cur->name, "parallax") == 0) 
	{
	  hotspot.para = XMLhelper::parse_int(doc, cur);
	}
      else
	{
	  std::cout << "XMLPLF: parse_hotspot: unhandeld: " << cur->name << std::endl;
	}
      cur = cur->next;
    }
  hotspots.push_back(hotspot);
}

/* EOF */
