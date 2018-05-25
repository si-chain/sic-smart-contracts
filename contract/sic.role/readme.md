#### Contract Introduction                                                                 
  * name: sic.role                                                     
  * introduction: Set the Special Account.
                                                                                                        
#### table                                                                
*(scope：used to get table)*                                                  
  * rolelist: the list of special account(scope: contract);                     
  * adminlist: the list of admin(scope: contract);                                         
                                                                          
#### action                                                               
  * setrole: set the account's role;                                                    
    * format: '["account","role type", "admin"]'；
    * auth: admin's permission;  
  * delrole: del the account's role;                                                    
    * format: '["account", "admin"]'；
    * auth: admin's permission;   
  * setadmin: set the admin;                                                    
    * format: '["admin account"]'；
    * auth: contract's permission;  
  * delrole: del the admin;                                                    
    * format: '["admin account"]'；
    * auth: contract's permission;  

#### Notes                                                                
About the role type:

  * 0 :  admin (only set by contract)                                                           
  * 1 :  reviewer                                                        
  * 2 :  company  
  * 3 :  developer