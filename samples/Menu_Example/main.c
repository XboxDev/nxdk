#include <hal/debug.h>
#include <hal/video.h>
#include <SDL.h>
#include <windows.h>
#include <pbkit/pbkit.h>
#include <stdbool.h>

//This is an example on how a menu with controller support could be created using NXDK

//This used the sdl_gamecontroller example as a template!

int push = 0; //1 if button is pushed 0, if button is not pushed
int menuptr = 1; //Menu Pointer, stores the position of the menu
int menu = 1; //Menu Number, stores which menu is displayed

//Menus
void menu1(int ptr){
    if(ptr == 1){pb_print("Option 1 <---\n");}
    else{pb_print("Option 1\n");}
    if(ptr == 2){pb_print("Option 2 <---\n");}
    else{pb_print("Option 2\n");}
    if(ptr == 3){pb_print("Option 3 <---\n");}
    else{pb_print("Option 3\n");}
    if(ptr == 4){pb_print("Quit <---\n");}
    else{pb_print("Quit\n");}
}

void menu2(int ptr){
    if(ptr == 1){pb_print("Option 4 <---\n");}
    else{pb_print("Option 4\n");}
    if(ptr == 2){pb_print("Option 5 <---\n");}
    else{pb_print("Option 5\n");}
    if(ptr == 3){pb_print("Option 6 <---\n");}
    else{pb_print("Option 6\n");}
    if(ptr == 4){pb_print("Back <---\n");}
    else{pb_print("Back\n");}
}

void menu3(int ptr){
    if(ptr == 1){pb_print("Option 7 <---\n");}
    else{pb_print("Option 7\n");}
    if(ptr == 2){pb_print("Option 8 <---\n");}
    else{pb_print("Option 8\n");}
    if(ptr == 3){pb_print("Option 9 <---\n");}
    else{pb_print("Option 9\n");}
    if(ptr == 4){pb_print("Option 10 <---\n");}
    else{pb_print("Option 10\n");}
    if(ptr == 5){pb_print("Back <---\n");}
    else{pb_print("Back\n");}
}

void menu4(int ptr){
    if(ptr == 1){pb_print("Option 11 <---\n");}
    else{pb_print("Option 11\n");}
    if(ptr == 2){pb_print("Option 12 <---\n");}
    else{pb_print("Option 12\n");}
    if(ptr == 3){pb_print("Back <---\n");}
    else{pb_print("Back\n");}
}



int main(void)
{
  SDL_GameController *pad = NULL;
  static SDL_Event e;
  bool pbk_init = false, sdl_init = false;

  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  sdl_init = SDL_Init(SDL_INIT_GAMECONTROLLER) == 0;
  if (!sdl_init) {
    debugPrint("SDL_Init failed: %s\n", SDL_GetError());
    goto wait_then_cleanup;
  }

  pbk_init = pb_init() == 0;
  if (!pbk_init) {
    debugPrint("pbkit init failed\n");
    goto wait_then_cleanup;
  }

  pb_show_front_screen();

  while (1) {
    pb_wait_for_vbl();
    pb_target_back_buffer();
    pb_reset();
    pb_fill(0, 0, 640, 480, 0);
    pb_erase_text_screen();

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_CONTROLLERDEVICEADDED) {
        SDL_GameController *new_pad = SDL_GameControllerOpen(e.cdevice.which);
        if (pad == NULL) {
          pad = new_pad;
        }
      }
      else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
        if (pad == SDL_GameControllerFromInstanceID(e.cdevice.which)) {
          pad = NULL;
        }
        SDL_GameControllerClose(SDL_GameControllerFromInstanceID(e.cdevice.which));
      }
      else if (e.type == SDL_CONTROLLERBUTTONDOWN) {
        if (e.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
          pad = (SDL_GameControllerFromInstanceID(e.cdevice.which));
        }
      }
    }

    SDL_GameControllerUpdate();
//Define Buttons
    auto DLEFT = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT); //DPAD Left Button
    auto DRIGHT = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT); //DPAD Right Button
    auto DUP = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_UP); //DPAD Up Button
    auto DDOWN = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN); //DPAD Down Button
    auto A = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_A); //A button
    auto B = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_B); //B button
    auto Y = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_Y); //Y button
    auto X = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_X); //X button
    auto START = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_START); //Start button
    auto BACK = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_BACK); //Back button
    auto WHITEE = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER); //White button
    auto BLACKK = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER); //Black button
    auto RIGHT_TRIGGER = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT); // Right Trigger button
    auto LEFTT_RIGGER = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT); // LEFT Trigger button
    auto LEFT_STICK_X = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX); // Left Thumb Stick X POS
    auto LEFT_STICK_Y = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY); // Left Thumb Stick Y POS
    auto LEFT_STICK_BUTTON = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSTICK); //Left Thumb Stick button (Click Down)
    auto RIGHT_STICK_X = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTX); // Right Thumb Stick X POS
    auto RIGHT_STICK_Y = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTY); // Right Thumb Stick Y POS
    auto RIGHT_STICK_BUTTON = SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSTICK); //Right Thumb Stick button (Click Down)

    if (pad == NULL) {
      pb_print("Plug a controller in and press start!\n");
    }
    else {
        if(DDOWN != NULL){
            if(push == 0){//This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.
                //Scrolling
                menuptr = menuptr + 1;
                if(menu == 1){ //DPAD Down's action while on Menu 1
                    if(menuptr > 4){menuptr = 1;} //Set menuptr to 4 to disable infinity scrolling
                }
                if(menu == 2){ //DPAD Down's action while on Menu 2
                    if(menuptr > 4){menuptr = 1;} //Set menuptr to 4 to disable infinity scrolling
                }
                if(menu == 3){ //DPAD Down's action while on Menu 3
                    if(menuptr > 5){menuptr = 1;} //Set menuptr to 5 to disable infinity scrolling
                }
                if(menu == 4){ //DPAD Down's action while on Menu 4
                    if(menuptr > 3){menuptr = 1;} //Set menuptr to 3 to disable infinity scrolling
                }
            }
            push = push + 1; //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.
        }
        else if(DUP != NULL){
            if(push == 0){ //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.
                //Scrolling
                menuptr = menuptr - 1;
                if(menu == 1){ //DPAD Up's action while on Menu 1
                    if(menuptr < 1){menuptr = 4;} //Set menuptr to 1 to disable infinity scrolling
                }
                if(menu == 2){ //DPAD Up's action while on Menu 2
                    if(menuptr < 1){menuptr = 4;} //Set menuptr to 1 to disable infinity scrolling
                }
                if(menu == 3){ //DPAD Up's action while on Menu 3
                    if(menuptr < 1){menuptr = 5;} //Set menuptr to 1 to disable infinity scrolling
                }
                if(menu == 4){ //DPAD Up's action while on Menu 4
                    if(menuptr < 1){menuptr = 3;} //Set menuptr to 1 to disable infinity scrolling
                }
            }
            push = push + 1; //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.
        }
        else if(A != NULL){
            if(push == 0){ //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.            
                if(menu == 1){ //A's action while on Menu 1
                    if(menuptr == 1){
                        menuptr = 1;
                        menu = 2;
                    }
                    if(menuptr == 2){
                        menuptr = 1;
                        menu = 3;             
                    }
                    if(menuptr == 3){
                        menuptr = 1;
                        menu = 4;
                    }
                    if(menuptr == 4){return 1;}
                }
                else if(menu == 2){ //A's action while on Menu 2
                    if(menuptr == 4){
                        menuptr = 1;
                        menu = 1;
                    }
                }
                else if(menu == 3){ //A's action while on Menu 3
                    if(menuptr == 5){
                        menuptr = 1;
                        menu = 1;                    
                    }
                }
                else if(menu == 4){ //A's action while on Menu 4
                    if(menuptr == 3){
                        menuptr = 1;
                        menu = 1;                    
                    }
                }
            }
            push = push + 1; //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.
        }
        //B Button Action
        else if(B != NULL){
            if(push == 0){ //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.
                if(menu == 2){ //B's action while on Menu 2      
                    menuptr = 1;
                    menu = 1;
                }
                else if(menu == 3){ //B's action while on Menu 3        
                    menuptr = 1;
                    menu = 1;
                }
                else if(menu == 4){ //B's action while on Menu 4        
                    menuptr = 1;
                    menu = 1;
                }
            }
            push = push + 1; //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.
        }
        else{push = 0;} //This is to make sure it only does the action once while the button is held down. If removed, the action will loop until the button is unpressed.

        pb_print("Menu Example\n"); //All Menu Titles
        pb_print("------------\n"); //All Menu Titles

//Displays a menu based off what int is stored in the menu variable.
        if(menu == 1){
            menu1(menuptr);
        }
        else if(menu == 2){
            menu2(menuptr);        
        }
        else if(menu == 3){
            menu3(menuptr);        
        }
        else if(menu == 4){
            menu4(menuptr);        
        }
    }
    pb_draw_text_screen();
    while (pb_busy());
    while (pb_finished());
  }

wait_then_cleanup:
  Sleep(5000);

cleanup:
  if (pbk_init) {
    pb_kill();
  }
  if (pad != NULL) {
    SDL_GameControllerClose(pad);
  }
  if (sdl_init) {
    SDL_Quit();
  }
  
  return 0;
}
