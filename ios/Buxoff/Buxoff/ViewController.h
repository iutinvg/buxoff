//
//  ViewController.h
//  Buxoff
//
//  Created by Viacheslav Iutin on 30/07/2016.
//  Copyright © 2016 Viacheslav Iutin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <UITextFieldDelegate>

@property (nonatomic, strong) IBOutlet UITextField *textAmount;
@property (nonatomic, strong) IBOutlet UITextField *textDesc;
@property (nonatomic, strong) IBOutlet UITextField *textTags;
@property (nonatomic, strong) IBOutlet UITextField *textAcct;
@property (nonatomic, strong) IBOutlet UITextField *textEmail;

@property (nonatomic, strong) IBOutlet UIButton *buttonAdd;
@property (nonatomic, strong) IBOutlet UIButton *buttonPush;

@property (nonatomic, strong) IBOutlet UILabel *labelCounter;

- (IBAction)actionAdd:(id)sender;
- (IBAction)actionPush:(id)sender;
- (IBAction)textFieldDidChange:(UITextField*)f;

@end
