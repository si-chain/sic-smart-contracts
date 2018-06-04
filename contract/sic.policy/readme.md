#### Contract Introduction                                                                 
  * name: sic.policy                                                      
  * introduction: User can use this contract to upload policy, and reviewed by reviewer.
                                          
                                                                          
#### table                                                                
*(scope：used to get table)*                                                  
  * account: get the policy's count(scope: sic.policy);                     
  * policy：check the status of policy(scope: uploader)                                         
  * unapproved：the policy info which unapproved(scope: sic.policy)                       
  * reviewlist：the ID list of policy that have be review by reviewer (scope: reviewer)
                                                                          
#### action                                                               
  * upload: User upload the polciy;                                                    
    * format: '["applyID","uploader", "The list of OSS address", "file's count"]'；
    * auth: user's permission;                                                            
  * reviewing：reviewer claim the policy;                                                 
    * format: '["applyID","reviewer"]';                                            
    * auth: reviewer's permission;                                                             
  * approved: approve the policy and save the policy info:
    * format: '["applyID","policyID","policy info saved by JSON", "uploader", "index type", "index1", "index2", "index3", "index4", "index5"]';        
    * auth: reviewer's permission;                                                   
  * disapprove：disapprove the policy:                                                 
    * format: '["applyID","reason"]'；                                              
    * auth: reviewer's permission;                                                 
                                                                          
#### Notes                                                                
About the status of policy                                                       
  * 0 :  upload                                                           
  * 10 : reviewing                                                        
  * 20 : approved                                                         
  * 30 : disapprove 
