//  $Id: start_screen.cxx,v 1.22 2003/10/21 21:37:06 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "gui/gui_manager.hpp"
#include "gui/surface_button.hpp"
#include "gui/component.hpp"
#include "gui/screen_manager.hpp"
#include "blitter.hpp"
#include "gettext.h"
#include "game_session.hpp"
#include "globals.hpp"
#include "system.hpp"
#include "fonts.hpp"
#include "resource.hpp"
#include "start_screen.hpp"
#include "game_time.hpp"
#include "sound/sound.hpp"
#include "pingus_level.hpp"
#include "string_format.hpp"
#include "gui/display.hpp"

class StartScreenComponent : public GUI::Component
{
private:
  PingusLevel plf;
  Sprite background;
  std::string time_str;
  std::string description;

public:
  StartScreenComponent(const PingusLevel& plf);
  void draw(DrawingContext& gc);
  virtual ~StartScreenComponent() {}

private:
  const std::string& format_description(int length);
};

class StartScreenOkButton : public GUI::SurfaceButton
{
private:
  StartScreen* parent;
public:
  StartScreenOkButton(StartScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 + 225,
                         Display::get_height()/2 + 125,
                         ResDescriptor("core/start/ok"),
                         ResDescriptor("core/start/ok_clicked"),
                         ResDescriptor("core/start/ok_hover")),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, (float)x_pos + 32, (float)y_pos - 17, _("Ok"));
  }

  void on_click()
  {
    Sound::PingusSound::play_sound("yipee");
    parent->start_game();
  }


  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};


class StartScreenAbortButton
  : public GUI::SurfaceButton
{
private:
  StartScreen* parent;
public:
  StartScreenAbortButton(StartScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 - 278,
                         Display::get_height()/2 + 144,
                         ResDescriptor("core/start/back"),
                         ResDescriptor("core/start/back_clicked"),
                         ResDescriptor("core/start/back_hover")),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, (float)x_pos + 55, (float)y_pos, _("Abort"));
  }

  void on_click() {
    parent->cancel_game();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};

StartScreen::~StartScreen()
{

}

StartScreenComponent::StartScreenComponent(const PingusLevel& p)
  : plf(p)
{
  if (Display::get_width() == 800 && Display::get_height() == 600)
    {
      background = Resource::load_sprite("core/menu/startscreenbg");
    }
  else
    {
#if 0
      PixelBuffer pb = Blitter::scale_surface_to_canvas(Resource::load_pixelbuffer(
                                                                                      "core/menu/startscreenbg"), Display::get_width(), Display::get_height());
      CL_SpriteDescription desc;
      desc.add_frame(pb);
      background = CL_Sprite(desc);
#endif
    }
  ////background.set_alignment(origin_center);
  time_str = GameTime::ticks_to_realtime_string(plf.get_time());
}

void
StartScreenComponent::draw(DrawingContext& gc)
{
  gc.draw(background, Display::get_width()/2, Display::get_height()/2);

#if 0
  int left_x  = Display::get_width()/2 - 120;
  int right_x = Display::get_width()/2 + 120;
  int y = Display::get_height()/2 + 40;

  gc.print_center(Fonts::chalk_large,
                  gc.get_width()/2,
                  (float)Display::get_height()/2 - 200,
                  _(plf.get_levelname()));

  gc.print_left(Fonts::chalk_normal,
                (float)Display::get_width()/2 - 290,
                (float)Display::get_height()/2 - 140,
                format_description(800 - 230));

  gc.print_left (Fonts::chalk_normal, (float)left_x,  (float)y, _("Number of Pingus: "));
  gc.print_right(Fonts::chalk_normal, (float)right_x, (float)y, CL_String::to(plf.get_number_of_pingus()));

  gc.print_left (Fonts::chalk_normal, (float)left_x,  float(y += 30), _("Number to Save: "));
  gc.print_right(Fonts::chalk_normal, (float)right_x, (float)y, CL_String::to(plf.get_number_to_save()));

  gc.print_left (Fonts::chalk_normal, (float)left_x,  float(y += 30), _("Time: "));
  gc.print_right(Fonts::chalk_normal, (float)right_x, (float)y, time_str);

  gc.print_left (Fonts::chalk_normal, (float)left_x,  float(y += 30), _("Difficulty:"));
  gc.print_right(Fonts::chalk_normal, (float)right_x, (float)y, CL_String::to(plf.get_difficulty()) + "/100");

  gc.print_center(Fonts::chalk_small, (float)Display::get_width()/2,
                  (float)Display::get_height()/2 + 270, _("Author: ") + plf.get_author());

  if (maintainer_mode)
    gc.print_left(Fonts::chalk_small, 110, 430, _("Filename: ") + plf.get_resname());

  CL_System::sleep(30);
#endif
}

const std::string&
StartScreenComponent::format_description(int length)
{
  if (description != "")
    return description;

  description = _(plf.get_description());

  if (description == "")
    return description;

  description = StringFormat::break_line(description, length, Fonts::chalk_normal);

  return description;
}


StartScreen::StartScreen(const PingusLevel& arg_plf)
  : plf(arg_plf)
{
  StartScreenComponent* comp = new StartScreenComponent(plf);
  gui_manager->add(comp, true);
  gui_manager->add(new StartScreenOkButton(this), true);
  gui_manager->add(new StartScreenAbortButton(this), true);
}

void
StartScreen::on_fast_forward_press()
{
  start_game();
}

void
StartScreen::on_pause_press ()
{
  start_game();
}

void
StartScreen::on_escape_press()
{
  cancel_game();
}

void
StartScreen::start_game()
{
  PingusGameSession* game_session = new PingusGameSession(plf, true);
  ScreenManager::instance()->replace_screen(game_session, true);
}

void
StartScreen::cancel_game()
{
  ScreenManager::instance()->pop_screen();
}


/* EOF */