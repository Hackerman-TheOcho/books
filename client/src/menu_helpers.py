'''menu helpers for books client'''
import os

format_line: str = "-" * 100

def singleton(cls):
    '''singleton decorator'''
    instances: dict = {}
    def get_instance(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]
    return get_instance

def banner():
    '''banner for client interface'''
    banner_text = r'''                                                                                                      
     ##### /              /                                                                            
  ######  /        #    #/                                           #                                 
 /#   /  /        ###   ##                                          ###                                
/    /  /          #    ##                                           #                                 
    /  /                ##                                                                             
   ## ##         ###    ## /###   ###  /###     /###   ###  /###   ###   ##   ####    ### /### /###    
   ## ##          ###   ##/ ###  / ###/ #### / / ###  / ###/ #### / ###   ##    ###  / ##/ ###/ /##  / 
   ## ##           ##   ##   ###/   ##   ###/ /   ###/   ##   ###/   ##   ##     ###/   ##  ###/ ###/  
   ## ##           ##   ##    ##    ##       ##    ##    ##          ##   ##      ##    ##   ##   ##   
   ## ##           ##   ##    ##    ##       ##    ##    ##          ##   ##      ##    ##   ##   ##   
   #  ##           ##   ##    ##    ##       ##    ##    ##          ##   ##      ##    ##   ##   ##   
      /            ##   ##    ##    ##       ##    ##    ##          ##   ##      ##    ##   ##   ##   
  /##/           / ##   ##    /#    ##       ##    /#    ##          ##   ##      /#    ##   ##   ##   
 /  ############/  ### / ####/      ###       ####/ ##   ###         ### / ######/ ##   ###  ###  ###  
/     #########     ##/   ###        ###       ###   ##   ###         ##/   #####   ##   ###  ###  ### 
#                                                                                                      
 ##                                                                                                    
                                                                                                                             
'''
    return banner_text

def clear():
    '''clear the screen'''
    os.system('cls' if os.name == 'nt' else 'clear')

def line_brackets(line: str) -> str:
    '''put brackets on a line and make it 100 characters wide'''
    new_line = f'{line:98}'
    return f'| {new_line}'

