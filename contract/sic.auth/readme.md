#### Contract Introduction                                                                 
  * name: sic.auth                                                     
  * introduction: Request the user authorization and user agree.
                                                                                                        
#### table                                                                
*(scope：used to get table)*                                                  
  * authlist: the list of request list(scope: user);                     
  * authstatus: the status of request(scope: company);                                         
                                                                          
#### action                                                               
  * reqauth: request the user's authorization;                                                    
    * format: '["requestID","user", "company", "encryption public key", "Authorization time"]'；
    * auth: company's permission;  
  * agreeauth: agree the request and save data;                                                    
    * format: '["requestID", "user", "encryption data"]'；
    * auth: user's permission;   
  * rejectauth: reject the request;                                                    
    * format: '["requestID", "user"]'；
    * auth: user's permission;  

#### Notes                                                                
About the status of request:

  * 0 :  unauthorized                                                        
  * 10 :  authorized                                                         
  * 2 :  rejected  
  * 3 :  time out

