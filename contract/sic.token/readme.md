#### Contract Introduction                                                                 
  * name: sic.token                                                      
  * introduction: The SIC token to reward user for upload policy and auth  
  * note：The SIC's decimal is 4, e.g. "10.0000 SIC"                                   
                                                                          
#### table                                                                
*(scope: used to get table)*                                                  
  * account：get the SIC info from this table (scope：user account)；                           
  * stat：all the info of this contract (scope：sic.token)       
  * special account: sic.hole, used to record the total SIC that user withdrawed                                                                                                           
#### action                                                               
  * issue：issue the SIC to the one account:                                                        
    * format: '["receiver","SIC's amount "]';                          
    * auth：contract's permission;                                                      
  * transfer： transfer to some one:                                                
    * format: '["from","to","SIC's amount"]';                                           
    * auth：from's permission;                                                       
  * withdraw：withdraw( only withdraw 1000 SIC everytime, and only withdraw once per 2 hours ) 
    * format: '["user","SIC's amount"]'；                        
    * auth: user's permission;                                                           
  * setfreeze：freeze one account and keep him from transfer withdraw:                            
    * format: '["freeze account", 0 or 1]';                                            
    * auth：contract's permission;
  * setallfreeze：freeze all the transfer and withdraw:
    * format: '[0 or 1]';
    * auth：contract's permission;
  * setwithdraw: freeze the withdraw:
    * format: '[0 or 1]';
    * auth：contract's permission;
