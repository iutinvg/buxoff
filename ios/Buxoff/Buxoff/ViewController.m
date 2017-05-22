//
//  ViewController.m
//  Buxoff
//
//  Created by Viacheslav Iutin on 30/07/2016.
//  Copyright © 2016 Viacheslav Iutin. All rights reserved.
//

#import "ViewController.h"
#import "RecordStorageWrapper.h"
#import "ViewHelpersWrapper.h"
#import "ControllerHelpersWrapper.h"
#import "UserDefaultsWrapper.h"
#import "EmailWrapper.h"

@interface ViewController ()
@property BOOL tagWasResolved;
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    _tagWasResolved = YES;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self updatePersistentFiedls];
    [self updateButtons];
}

#pragma - UI setup
- (void)updateButtons
{
    int count = RecordStorageWrapper.count;
    [_buttonAdd setEnabled:[ViewHelpersWrapper enableAdd:_textAmount.text
                                                 account:_textAcct.text]];
    [_buttonPush setEnabled:[ViewHelpersWrapper enablePush:count
                                                    amount:_textAmount.text
                                                   account:_textAcct.text
                                                     email:_textEmail.text]];
    _labelCounter.text = [NSString stringWithFormat:@"total records: %d", count];
}

- (void)updatePersistentFiedls
{
    for (UITextField *f in @[_textAcct, _textEmail]) {
        f.text = [UserDefaultsWrapper get:f.accessibilityLabel def:@""];
    }
}

- (void)updateUI
{
}

- (void)clearText
{
    for (UITextField *f in @[_textAmount, _textDesc, _textTags]) {
        f.text = @"";
    }
    [_textAmount becomeFirstResponder];
}

- (void)updatePersitentText:(UITextField*)f
{
    if (f.accessibilityLabel) {
        NSLog(@"save UD: %@->%@", f.accessibilityLabel, f.text);
        [UserDefaultsWrapper put:f.accessibilityLabel value:f.text];
    }
}

#pragma mark - Actions
- (void)actionAdd:(id)sender
{
    [ControllerHelpersWrapper add:_textAmount.text
                             desc:_textDesc.text
                              tag:_textTags.text
                          account:_textAcct.text];
    [self clearText];
    [self updateButtons];
}

- (void)actionPush:(id)sender
{
    NSString* body = [ControllerHelpersWrapper push:_textAmount.text
                                               desc:_textDesc.text
                                                tag:_textTags.text
                                            account:_textAcct.text];
    [self sendEmail:body];
    [self clearText];
    [self updateButtons];
}

#pragma mark - UITextView
- (void)textFieldDidChange:(UITextField*)f
{
    [self updateButtons];
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    [self updatePersitentText:textField];
    return YES;
}

#pragma mark - UIMailComposer
- (void)mailComposeController:(MFMailComposeViewController *)controller
          didFinishWithResult:(MFMailComposeResult)result
                        error:(NSError *)error
{
    [controller dismissViewControllerAnimated:YES completion:nil];
    if (result == MFMailComposeResultFailed) {
        [self sendingFailed];
    }
}

#pragma mark - Utils
- (void)sendEmail:(NSString*)body
{
    if (![MFMailComposeViewController canSendMail]) {
        [self sendingFailed];
        return;
    }
    
    NSString* email = [UserDefaultsWrapper get:_textAmount.accessibilityLabel
                                           def:@""];
    MFMailComposeViewController *c = [[MFMailComposeViewController alloc] init];
    c.mailComposeDelegate = self;
    [c setToRecipients:@[email]];
    [c setSubject:[EmailWrapper subject]];
    [c setMessageBody:body isHTML:NO];
    [self presentViewController:c animated:YES completion:nil];
}

- (void)sendingFailed
{
    [self showAlert:@"Error" body:@"Email sending is not setup"];
}

- (void)showAlert:(NSString*)title body:(NSString*)body
{
    UIAlertController *ac = [UIAlertController alertControllerWithTitle:title
                                                                message:body
                                                         preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *ok = [UIAlertAction actionWithTitle:@"OK"
                                                 style:UIAlertActionStyleDefault
                                               handler:nil];
    [ac addAction:ok];
    [self presentViewController:ac animated:YES completion:nil];
}

@end
