#### Contract Introduction   
  * name: license.list
  * introduction: To save the User License Agreement;

#### table
*(scope：used to get table)*
  * license：get the user's License Agreement(scope:user);
  * licenseinfo：get More information about License Agreement(scope: contract);

#### action
  * agree：user agree the License Agreement:
    * format: '["user account","version number's int", "Version number"]';
    * auth: user's permission;  
  * setlicense：upload the full info about the License Agreement:
    * format: '["version number's int", "Version number", "full info"]';
    * auth：contract's permission;  
